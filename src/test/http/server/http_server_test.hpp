#include "http/client/http_client.hpp"
#include "http/server/http_server.hpp"
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

TEST(HttpServer, StaticDir) {
  HttpServer server;
  Address addr{"127.0.0.1", 8080};
  auto rc = server.Init(addr);
  MUST_TRUE(rc == 0, server.err_msg());

  server.StaticDir("/", "./");

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
  req.GET("/index.html");
  HttpResp resp;
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
  DEBUG("resp: " << resp.body());

  req.Clear();
  req.GET("/index.htm");
  resp.Clear();
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::NOT_FOUND,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));

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
  server.Use([&count](HttpContext &ctx) {
    DEBUG("use middleware");
    count++;
    ctx.Next();
  });

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

  req.GET("/hello");
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
  DEBUG("resp: " << resp.body());

  MUST_TRUE(count == 1, "get wrong count " + to_string(count));
}
