#include "./cppnet/include/cppnet/http/server/http_server.hpp"
#include "./cppnet/include/cppnet/log/std_logger.hpp"
#include "./cppnet/include/cppnet/utils/const.hpp"
#include <iostream>
#include <memory>

using namespace cppnet;

int main() {
  HttpServer server;
  std::shared_ptr<SSLContext> ssl_ctx = std::make_shared<SSLContext>();
  auto rc = ssl_ctx->InitSvrFile("./ssl/cacert.pem", "./ssl/privkey.pem");
  if (rc != kSuccess) {
    std::cout << "init ssl context wrong " << ssl_ctx->err_msg() << std::endl;
    return rc;
  }
  rc = server.InitSSL({"127.0.0.1", 8080}, ssl_ctx);
  if (rc != kSuccess) {
    std::cout << "init server wrong " << server.err_msg() << std::endl;
    return rc;
  }
  server.set_logger(std::make_shared<StdLogger>());
  server.GET("/", [](HttpContext &ctx) {
    ctx.resp().Success(HttpHeader::ContentType::kTextHtml,
                       "<h1>Hello, World!</h1>");
  });
  rc = server.Run();
  if (rc != kSuccess) {
    std::cout << "run server wrong " << server.err_msg() << std::endl;
    return rc;
  }
  return 0;
}
