#include "http/client/http_client.hpp"
#include "http/server/http_server.hpp"
#include "log/std_logger.hpp"
#include "test.h"
#include "utils/file.hpp"
#include <atomic>

using namespace cppnet;
using namespace std;

TEST(HttpServer, HelloWorld) {
  HttpServer server;
  Address addr{"127.0.0.1", 8080};
  auto rc = server.Init(addr);
  MUST_TRUE(rc == 0, server.err_msg());

  server.GET("/hello", [&](HttpContext &ctx) {
    ctx.resp().Text(HttpStatusCode::OK, "hello world");
    server.Stop();
  });

  // sync run server
  GO_JOIN([&] { server.Run(); });

  HttpClient client;
  rc = client.Init(addr);
  MUST_TRUE(rc == 0, client.err_msg());

  HttpReq req;
  HttpResp resp;

  req.GET("/world");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::NOT_FOUND,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));

  req.Clear();
  resp.Clear();
  req.GET("/hello");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
  DEBUG("resp: " << resp.body());
}

TEST(HttpServer, Static) {
  HttpServer server;
  Address addr{"127.0.0.1", 8080};
  auto rc = server.Init(addr);
  MUST_TRUE(rc == 0, server.err_msg());
  server.set_logger(std::make_shared<StdLogger>());

  server.StaticDir("/static", "./");
  server.StaticFile("/index.h", "./index.html");

  server.StaticDir("/test/", "./");
  server.StaticFile("/test/", "./index.html");

  // create file
  rc = File::Write("./index.html", "hello world");
  MUST_TRUE(rc == 0, "write file failed");
  DEFER([]() { remove("./index.html"); });

  // sync run server
  GO_JOIN([&] { server.Run(); });

  HttpClient client;
  rc = client.Init(addr);
  MUST_TRUE(rc == 0, client.err_msg());

  HttpReq req;
  req.GET("/static/index.html");
  HttpResp resp;
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
  MUST_TRUE(resp.header().GetContentType() ==
                HttpHeader::ContentType::kTextHtml,
            HttpHeader::ConvertToStr(resp.header().GetContentType()));
  DEBUG("resp: " << resp.body());

  req.Clear();
  req.GET("/index.ht");
  resp.Clear();
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::NOT_FOUND,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));

  req.Clear();
  req.GET("/index.h");
  resp.Clear();
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.header().GetContentType() ==
                HttpHeader::ContentType::kTextHtml,
            HttpHeader::ConvertToStr(resp.header().GetContentType()));

  req.Clear();
  req.GET("/test/index.html");
  resp.Clear();
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());

  req.Clear();
  req.GET("/test/");
  resp.Clear();
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());

  server.Stop();
}

TEST(HttpServer, Group) {
  HttpServer server;
  Address addr{"127.0.0.1", 8080};
  auto rc = server.Init(addr);
  MUST_TRUE(rc == 0, server.err_msg());

  auto api_group = server.Group("/api");
  {
    api_group.GET("/hello", [&](HttpContext &ctx) {
      ctx.resp().Text(HttpStatusCode::OK, "hello world");
      server.Stop();
    });
  }

  // sync run server
  GO_JOIN([&] { server.Run(); });

  HttpClient client;
  rc = client.Init(addr);
  MUST_TRUE(rc == 0, client.err_msg());

  HttpReq req;
  HttpResp resp;

  req.GET("/hello");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::NOT_FOUND,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));

  req.Clear();
  resp.Clear();
  req.GET("/api/hello");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
}

TEST(HttpServer, Middleware) {
  HttpServer server;
  Address addr{"127.0.0.1", 8080};
  auto rc = server.Init(addr);
  MUST_TRUE(rc == 0, server.err_msg());

  atomic<int> count = 0;
  auto count_func = [&count](HttpContext &ctx) {
    DEBUG("use count");
    count++;
    ctx.Next();
  };

  server.Use(count_func);

  server.GET("/hello", {count_func,
                        [&](HttpContext &ctx) {
                          ctx.resp().Text(HttpStatusCode::OK, "hello world");
                          server.Stop();
                        },
                        count_func});

  // sync run server
  GO_JOIN([&] { server.Run(); });

  HttpClient client;
  rc = client.Init(addr);
  MUST_TRUE(rc == 0, client.err_msg());

  HttpReq req;
  HttpResp resp;

  req.GET("/hello");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
  DEBUG("resp: " << resp.body());

  MUST_TRUE(count == 2, "get wrong count " + to_string(count));
}

#ifdef CPPNET_OPENSSL
TEST(HttpServer, HttpsServer) {
  HttpServer server;
  server.set_logger(std::make_shared<StdLogger>());
  Address addr{"127.0.0.1", 8080};
  std::shared_ptr<SSLContext> ssl_context = std::make_shared<SSLContext>();
  auto rc = ssl_context->InitSvrFile("./test/ssl/cert_demo.pem",
                                     "./test/ssl/cert_key.pem");
  MUST_TRUE(rc == 0, ssl_context->err_msg());
  rc = server.InitSSL(addr, ssl_context);
  MUST_TRUE(rc == 0, server.err_msg());

  server.GET("/hello", [&](HttpContext &ctx) {
    ctx.resp().Text(HttpStatusCode::OK, "hello world");
    server.Stop();
  });

  // sync run server
  GO_JOIN([&] { server.Run(); });

  HttpClient client;
  rc = client.InitSSL(addr);
  MUST_TRUE(rc == 0, client.err_msg());

  HttpReq req;
  HttpResp resp;

  req.GET("/hello");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
  DEBUG("resp: " << resp.body());
  client.Close();
  server.Stop();
}
#endif
