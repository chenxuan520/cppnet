#include "http/client/http_client.hpp"
#include "http/server/http_server.hpp"
#include "log/std_logger.hpp"
#include "test.h"
#include "utils/file.hpp"
#include <atomic>
#include <chrono>
#include <thread>
#include <string>

using namespace cppnet;
using namespace std;

TEST(HttpServer, HelloWorld) {
  HttpServer server;
  Address addr{"127.0.0.1", 19981};
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

TEST(HttpServer, MethodFilter) {
  HttpServer server;
  Address addr{"127.0.0.1", 19982};
  auto rc = server.Init(addr);
  MUST_TRUE(rc == 0, server.err_msg());

  server.GET("/only-get", [&](HttpContext &ctx) {
    ctx.resp().Text(HttpStatusCode::OK, "get");
    server.Stop();
  });
  server.POST("/only-post", [&](HttpContext &ctx) {
    ctx.resp().Text(HttpStatusCode::OK, "post");
  });

  // sync run server
  GO_JOIN([&] { server.Run(); });

  HttpClient client;
  rc = client.Init(addr);
  MUST_TRUE(rc == 0, client.err_msg());
  HttpReq req;
  HttpResp resp;

  // a GET route must not accept POST
  req.POST("/only-get", "x");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::NOT_FOUND,
            "POST on GET route got " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));

  // a POST route must not accept GET
  req.Clear();
  resp.Clear();
  req.GET("/only-post");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::NOT_FOUND,
            "GET on POST route got " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));

  // matching methods still work
  req.Clear();
  resp.Clear();
  req.GET("/only-get");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.body() == "get", "get wrong body " + resp.body());
  client.Close();
}

TEST(HttpServer, ShortGarbageRequest) {
  HttpServer server;
  Address addr{"127.0.0.1", 19983};
  auto rc = server.Init(addr);
  MUST_TRUE(rc == 0, server.err_msg());

  server.GET("/hello", [&](HttpContext &ctx) {
    ctx.resp().Text(HttpStatusCode::OK, "hello world");
    server.Stop();
  });

  // sync run server
  GO_JOIN([&] { server.Run(); });

  // a lone "\n" (shorter than "\r\n\r\n") used to make ReadUntil throw
  // std::out_of_range and terminate the whole process
  for (auto garbage : {"\n", "\r\n", "\n\n\n"}) {
    Socket soc;
    rc = soc.Init();
    MUST_TRUE(rc == 0, soc.err_msg());
    rc = soc.Connect(addr);
    MUST_TRUE(rc == 0, soc.err_msg());
    soc.Write(garbage);
    soc.Close();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  // server must still be alive and serving
  HttpClient client;
  rc = client.Init(addr);
  MUST_TRUE(rc == 0, client.err_msg());
  HttpReq req;
  HttpResp resp;
  req.GET("/hello");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
  client.Close();
}

TEST(HttpServer, Static) {
  HttpServer server;
#ifdef _WIN32
  Address addr{"127.0.0.1", (uint16_t)(rand() % 1000 + 8000)};
#else
  Address addr{"127.0.0.1", 19981};
#endif
  auto rc = server.Init(addr);
  MUST_TRUE(rc == 0, server.err_msg());
  server.set_logger(std::make_shared<StdLogger>());

  server.StaticDir("/static", "./");
  server.StaticFile("/index.h", "./index.html");

  server.StaticDir("/test/", "./");
  server.StaticFile("/test/", "./index.html");
  server.StaticFile("/demo", "./index.demo");

  // create file
  rc = File::Write("./index.html", "hello world");
  MUST_TRUE(rc == 0, "write file failed");
  DEFER([]() { File::Remove("./index.html"); });

  rc = File::Write("./index.demo", "hello world");
  MUST_TRUE(rc == 0, "write file failed");
  DEFER([]() { File::Remove("./index.demo"); });

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

  // test custom type
  req.Clear();
  req.GET("/demo");
  resp.Clear();
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
  MUST_TRUE(resp.header().GetContentType() ==
                HttpHeader::ContentType::kApplicationOctetStream,
            "get wrong content type " +
                HttpHeader::ConvertToStr(resp.header().GetContentType()));

  HttpHeader::SetCustomContentType("demo", "application/demo");
  req.Clear();
  req.GET("/demo");
  resp.Clear();
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
  MUST_TRUE(resp.header()["Content-Type"] == "application/demo",
            "get wrong content type " +
                HttpHeader::ConvertToStr(resp.header().GetContentType()));
  server.Stop();
}

TEST(HttpServer, Group) {
#ifdef _WIN32
  Address addr{"127.0.0.1", (uint16_t)(rand() % 1000 + 8000)};
#else
  Address addr{"127.0.0.1", 19981};
#endif
  HttpServer server;
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
#ifdef _WIN32
  Address addr{"127.0.0.1", (uint16_t)(rand() % 1000 + 8000)};
#else
  Address addr{"127.0.0.1", 19981};
#endif
  HttpServer server;
  auto rc = server.Init(addr);
  MUST_TRUE(rc == 0, server.err_msg());

  int temp_var = 10;
  atomic<int> count = 0;
  auto count_func = [&count, &temp_var](HttpContext &ctx) {
    DEBUG("use count");
    count++;
    ctx.Set("temp", temp_var);
    ctx.Next();
    temp_var = ctx.Get<int>("temp");
  };
  atomic<int> abort_count = 0;
  auto abort_func = [&abort_count](HttpContext &ctx) {
    DEBUG("use abort");
    abort_count++;
    ctx.Abort();
  };

  server.Use(count_func);

  server.GET("/hello",
             {count_func, [&](HttpContext &ctx) {
                ctx.resp().Text(HttpStatusCode::OK, "hello world");
                auto val = ctx.Get<int>("temp");
                MUST_TRUE(val == 10, "get wrong value " + to_string(val));
                auto str_tmp = ctx.Get<std::string>("temp");
                MUST_TRUE(str_tmp == "", "get wrong value " + str_tmp);
                ctx.Set("temp", 20);
                DEBUG("temp: " << ctx.Get<int>("temp"));
                server.Stop();
              }});

  server.GET("/abort", {[&](HttpContext &ctx) {
                          ctx.resp().Text(HttpStatusCode::OK, "hello world");
                        },
                        abort_func,
                        [&](HttpContext &ctx) { FATAL("not end after abort"); },
                        abort_func});

  // sync run server
  GO_JOIN([&] { server.Run(); });

  HttpClient client;
  rc = client.Init(addr);
  MUST_TRUE(rc == 0, client.err_msg());

  HttpReq req;
  HttpResp resp;

  req.GET("/abort");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));

  req.GET("/hello");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
  DEBUG("resp: " << resp.body());

  MUST_TRUE(count == 3, "get wrong count " + to_string(count));
  MUST_TRUE(abort_count == 1,
            "get wrong abort count " + to_string(abort_count));
  MUST_TRUE(temp_var == 20, "temp_var is " + to_string(temp_var) + " not 20");
}

#ifdef CPPNET_OPENSSL
TEST(HttpServer, HttpsServer) {
  for (int i = 0; i < 2; i++) {
    int rc = 0;
    HttpServer server;
    switch (i) {
    case 0:
      server.server().set_mode(TcpServer::kIOMultiplexing);
      break;
    case 1:
      server.server().set_mode(TcpServer::kMultiThread);
      break;
      return;
    }
    server.set_logger(std::make_shared<StdLogger>());
    Address addr{"127.0.0.1", 19981};
    std::shared_ptr<SSLContext> ssl_context = std::make_shared<SSLContext>();
    rc = ssl_context->InitSvrFile("./test/ssl/cert_demo.pem",
                                  "./test/ssl/cert_key.pem");
    MUST_TRUE(rc == 0, ssl_context->err_msg());
    rc = server.InitSSL(addr, ssl_context);
    MUST_TRUE(rc == 0, server.err_msg());

    server.GET("/hello", [&](HttpContext &ctx) {
      ctx.resp().Text(HttpStatusCode::OK, "hello world");
      // server.Stop();
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
    DEBUG("client ssl send");
    MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
              "get wrong status code " +
                  HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
    MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
    DEBUG("resp: " << resp.body());

    // test timeout
    server.SetReadTimeout(0, 100000);
    SSLContext cli_ctx;
    rc = cli_ctx.InitCli();
    MUST_TRUE(rc == 0, cli_ctx.err_msg());
    auto soc_ssl = cli_ctx.CreateSSLSocket();
    rc = soc_ssl->Connect(addr);
    MUST_TRUE(rc == 0, soc_ssl->err_msg());
    rc = soc_ssl->Write("hello");
    MUST_TRUE(rc == 5, soc_ssl->err_msg());
    string tmp;
    soc_ssl->Read(tmp, 100);
    DEBUG(tmp);
    soc_ssl->Close();
    cli_ctx.Close();

    // test accept ssl timeout
    Socket soc;
    rc = soc.Init();
    MUST_TRUE(rc == 0, soc.err_msg());
    rc = soc.Connect(addr);
    MUST_TRUE(rc == 0, soc.err_msg());
    rc = soc.Write("hello");
    MUST_TRUE(rc == 5, soc.err_msg());
    soc.Read(tmp, 100);
    DEBUG(tmp);
    soc.Close();

    client.Close();
    server.Stop();
  }
}
#endif
