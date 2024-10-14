#pragma once
#include <unistd.h>
#ifdef CPPNET_OPENSSL

#include "ssl/ssl_context.hpp"
#include "test.h"

using namespace std;
using namespace cppnet;

TEST(SSLContext, SSLConnect) {
  SSLContext ssl_ctx;
  Address addr{"127.0.0.1", 8080};
  auto rc =
      ssl_ctx.InitFile("./test/ssl/cert_demo.pem", "./test/ssl/cert_key.pem");
  MUST_TRUE(rc == 0, ssl_ctx.err_msg());

  // recv_soc
  auto recv_ssl_sock = ssl_ctx.CreateSSLSocket();
  MUST_TRUE(recv_ssl_sock != nullptr, ssl_ctx.err_msg());
  MUST_TRUE(recv_ssl_sock->err_msg() == "", recv_ssl_sock->err_msg());

  recv_ssl_sock->SetReuseAddr();
  DEFER_DEFAULT { recv_ssl_sock->Close(); };

  MUST_TRUE(rc == 0, recv_ssl_sock->err_msg());
  rc = recv_ssl_sock->Bind(addr);
  MUST_TRUE(rc == 0, recv_ssl_sock->err_msg());
  rc = recv_ssl_sock->Listen(10);
  MUST_TRUE(rc == 0, recv_ssl_sock->err_msg());

  // send_soc
  GO_JOIN([&]() {
    SSLContext cli_ctx;
    auto rc = cli_ctx.InitCli();
    MUST_TRUE(rc == 0, cli_ctx.err_msg());

    Socket tmp_soc;
    tmp_soc.Init();
    auto send_ssl_sock = cli_ctx.CreateSSLSocket(tmp_soc);
    MUST_TRUE(send_ssl_sock != nullptr, cli_ctx.err_msg());
    rc = send_ssl_sock->Connect(addr);
    MUST_TRUE(rc == 0, send_ssl_sock->err_msg());
    DEBUG("send_sock connected");

    std::string send_msg = "hello";
    rc = send_ssl_sock->Write(send_msg);
    DEBUG("send_msg: " << send_msg);
    MUST_TRUE(rc == 5, send_ssl_sock->err_msg());

    // usleep(100);
    send_ssl_sock->Close();
    DEBUG("send_sock closed");
  });

  auto recv_soc = recv_ssl_sock->AcceptSSL(addr);
  MUST_TRUE(recv_soc->status() == Socket::kInit, recv_ssl_sock->err_msg());
  DEBUG("recv_sock accepted");

  std::string recv_msg;
  recv_soc->Read(recv_msg, 5);
  DEBUG("recv_msg: " << recv_msg);
  MUST_TRUE(recv_msg == "hello", "recv data error " + recv_soc->err_msg());
}

#endif
