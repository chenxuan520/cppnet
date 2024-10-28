#include "http/client/http_client.hpp"
#include "http/server/http_server.hpp"
#include "test.h"

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
  req.GET("/hello");
  HttpResp resp;
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "hello world", "get wrong body " + resp.body());
  DEBUG("resp: " << resp.body());
}
