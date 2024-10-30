#pragma once
#include "server/tcp_server.hpp"
#include "utils/const.hpp"
#include "utils/file.hpp"
#include <memory>
#include <unistd.h>
#include <unordered_map>
#ifdef CPPNET_OPENSSL

#include "ssl/ssl_context.hpp"
#include "test.h"

using namespace std;
using namespace cppnet;

TEST(SSLContext, SSLConnect) {
  SSLContext ssl_ctx;
  Address addr{"127.0.0.1", 8080};
  auto rc = ssl_ctx.InitSvrFile("./test/ssl/cert_demo.pem",
                                "./test/ssl/cert_key.pem");
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

  auto new_socket = recv_ssl_sock->Accept(addr);
  MUST_TRUE(new_socket.status() == Socket::kInit, recv_ssl_sock->err_msg());
  auto recv_soc = ssl_ctx.AcceptSSL(new_socket);
  MUST_TRUE(recv_soc->status() == Socket::kInit, recv_ssl_sock->err_msg());
  DEBUG("recv_sock accepted");

  std::string recv_msg;
  recv_soc->Read(recv_msg, 5);
  DEBUG("recv_msg: " << recv_msg);
  MUST_TRUE(recv_msg == "hello", "recv data error " + recv_soc->err_msg());
}

TEST(SSLContext, SSLConnectData) {
  // read from cert file
  std::string cert_data;
  std::string key_data;
  int rc = 0;

  rc = File::Read("./test/ssl/cert_demo.pem", cert_data);
  MUST_TRUE(rc == kSuccess, "read cert file failed");
  rc = File::Read("./test/ssl/cert_key.pem", key_data);
  MUST_TRUE(rc == kSuccess, "read key file failed");

  SSLContext ssl_ctx;
  rc = ssl_ctx.InitSvr(cert_data, key_data);
  MUST_TRUE(rc == kSuccess, ssl_ctx.err_msg());
}

TEST(SSLContext, SSLServer) {
  Address addr{"127.0.0.1", 8080};
  string msg = "hello";

  TcpServer server(addr);
  auto rc = server.Init();
  MUST_TRUE(rc == 0, server.err_msg());

  // init ssl context
  SSLContext ssl_ctx;
  rc = ssl_ctx.InitSvrFile("./test/ssl/cert_demo.pem",
                           "./test/ssl/cert_key.pem");
  MUST_TRUE(rc == 0, ssl_ctx.err_msg());

  unordered_map<int, std::shared_ptr<SSLSocket>> hashmap;

  // event call func
  auto event_func = [&](TcpServer::Event event, TcpServer &, Socket fd) {
    switch (event) {
    case TcpServer::Event::kEventAccept: {
      auto ssl_ptr = ssl_ctx.AcceptSSL(fd);
      DEBUG(fd.fd() << " accept ssl sock");
      hashmap[fd.fd()] = ssl_ptr;
    } break;

    case TcpServer::Event::kEventRead: {
      auto it = hashmap.find(fd.fd());
      MUST_TRUE(it != hashmap.end(), "not found fd" << fd.fd());
      auto ssl_sock = it->second;
      std::string buf;
      auto rc = ssl_sock->Read(buf, 5, true);
      DEBUG(fd.fd() << " read " << buf);
      MUST_TRUE(buf == msg, ssl_sock->err_msg());
    } break;

    case TcpServer::Event::kEventLeave: {
      auto it = hashmap.find(fd.fd());
      MUST_TRUE(it != hashmap.end(), "not found fd");
      auto ssl_sock = it->second;
      ssl_sock->CloseSSL();
      DEBUG(fd.fd() << " close");
      hashmap.erase(it);
      server.Stop();
    } break;

    case TcpServer::Event::kEventError: {
      FATAL(fd.err_msg());
      server.Stop();
      break;
    }
    }
  };

  // thread run client
  GO_JOIN([&]() {
    SSLContext cli_ctx;
    auto rc = cli_ctx.InitCli();
    MUST_TRUE(rc == 0, cli_ctx.err_msg());

    auto send_ssl_sock = cli_ctx.CreateSSLSocket();
    MUST_TRUE(send_ssl_sock != nullptr, cli_ctx.err_msg());
    DEBUG("send_sock created");
    rc = send_ssl_sock->Connect(addr);
    MUST_TRUE(rc == 0, send_ssl_sock->err_msg());
    DEBUG("send_sock connected");

    rc = send_ssl_sock->Write(msg);
    MUST_TRUE(rc == 5, send_ssl_sock->err_msg());
    DEBUG("send_msg: " << msg);

    usleep(100);
    send_ssl_sock->Close();
    DEBUG("send_sock closed");
  });

  // start server
  server.Register(event_func);
  rc = server.EventLoop();
  MUST_TRUE(rc == 0, server.err_msg());
}
#endif
