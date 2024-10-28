#include "http/server/http_server.hpp"
#include "test.h"

using namespace cppnet;
using namespace std;

TEST(HttpServer, HelloWorld) {
  HttpServer server;
  Address addr{"127.0.0.1", 8080};
  auto rc = server.Init(addr);
  MUST_TRUE(rc == 0, server.err_msg());

  server.GET("/hello", [](HttpContext &ctx) {
    ctx.resp().Text(HttpStatusCode::OK, "hello world");
  });
}
