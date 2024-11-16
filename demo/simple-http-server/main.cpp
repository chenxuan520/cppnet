#include "http/server/http_server.hpp"
#include "log/std_logger.hpp"
#include <iostream>

using namespace cppnet;
using namespace std;

int main() {
  HttpServer server;
  auto rc = server.Init({"127.0.0.1", 8080});
  // auto ssl_context = make_shared<SSLContext>();
  // auto rc =
  //     ssl_context->InitSvrFile("./ssl/cert_demo.pem", "./ssl/cert_key.pem");
  // if (rc != 0) {
  //   std::cout << "init ssl failed " << ssl_context->err_msg() << std::endl;
  //   return rc;
  // }
  // rc = server.InitSSL({"127.0.0.1", 8080}, ssl_context);
  if (rc != 0) {
    std::cout << "init server failed " << server.err_msg() << std::endl;
    return rc;
  }
  server.set_logger(std::make_shared<StdLogger>());

  server.GET("/hello", [&](HttpContext &ctx) {
    ctx.resp().Text(HttpStatusCode::OK, "hello world");
  });

  server.GET("/world", [](HttpContext &ctx) {
    ctx.resp().Redirect("https://chenxuanweb.top");
  });

  server.StaticDir("/", "./blog-home");
  server.StaticFile("/", "./blog-home/index.html");

  rc = server.Run();
  if (rc != 0) {
    std::cout << "run server failed" << server.err_msg() << std::endl;
    return rc;
  }
  cout << "3" << endl;
  return 0;
}
