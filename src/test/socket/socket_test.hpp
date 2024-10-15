#pragma once

#include "socket/socket.hpp"
#include "test.h"
#include <unistd.h>

using namespace cppnet;

TEST(Socket, UDP) {
  Address addr{"127.0.0.1", 8080};
  int rc = 0;

  Socket socket_recv, socket_send;
  rc = socket_recv.InitUdp();
  MUST_TRUE(rc == 0, strerror(errno));
  rc = socket_send.InitUdp();
  MUST_TRUE(rc == 0, strerror(errno));
  DEFER([&]() {
    socket_recv.Close();
    socket_send.Close();
  });

  // bind server
  rc = socket_recv.Bind(addr);
  MUST_TRUE(rc == 0, strerror(errno));
  socket_recv.SetReuseAddr();
  rc = socket_recv.SetReuseAddr();
  MUST_TRUE(rc == 0, strerror(errno));

  // send
  std::string msg = "hello world";
  rc = socket_send.WriteUdp(msg.c_str(), msg.size(), addr);
  MUST_TRUE(rc > 0, strerror(errno));

  // recv
  char buf[1024] = {0};
  Address addr_recv;
  rc = socket_recv.ReadUdp(buf, sizeof(buf), addr_recv);
  MUST_TRUE(rc == msg.size(), strerror(errno));
  MUST_TRUE(msg == buf, strerror(errno));

  DEBUG("recv: " + std::string(buf));
}

TEST(Socket, CompleteRead) {
  Address addr{"127.0.0.1", 8081};
  int rc = 0;
  const int max_connect_queue = 10;
  std::string msg = "hello world";

  Socket socket_recv, socket_send;
  rc = socket_recv.Init();
  MUST_TRUE(rc == 0, strerror(errno));
  rc = socket_send.Init();
  MUST_TRUE(rc == 0, strerror(errno));
  DEFER([&]() {
    socket_recv.Close();
    socket_send.Close();
  });

  // bind
  rc = socket_recv.SetReuseAddr();
  MUST_TRUE(rc == 0, strerror(errno));
  rc = socket_recv.Bind(addr);
  MUST_TRUE(rc == 0, strerror(errno));

  // listen
  rc = socket_recv.Listen(max_connect_queue);
  MUST_TRUE(rc == 0, strerror(errno));

  // thread to run send func
  GO_JOIN([&]() {
    // connect
    auto cli_rc = socket_send.Connect(addr);
    MUST_TRUE(cli_rc == 0, strerror(errno));
    DEBUG("client connect");

    // write twice
    std::string msg_substr(msg, 0, 5);
    cli_rc = socket_send.Write(msg_substr);
    MUST_TRUE(cli_rc == msg_substr.size(), strerror(errno));
    DEBUG("client write " << msg_substr);
    usleep(1000);
    cli_rc = socket_send.Write(msg.substr(5));
    MUST_TRUE(cli_rc == msg.size() - msg_substr.size(), strerror(errno));
    DEBUG("client write " << msg.substr(5));

    // read back
    std::string buf;
    cli_rc = socket_send.Read(buf, msg.size());
    MUST_TRUE(cli_rc == msg.size(), strerror(errno));
    MUST_TRUE(msg == buf, strerror(errno));
    DEBUG("client read " << buf);
  });

  // accept
  Address addr_cli;
  Socket socket_accept = socket_recv.Accept(addr_cli);

  // read
  std::string msg_rec;
  rc = socket_accept.Read(msg_rec, msg.size(), true);
  DEBUG("read " << msg_rec);
  MUST_EQUAL(msg, msg_rec);

  // write
  rc = socket_accept.Write(msg_rec);
  MUST_TRUE(rc == msg_rec.size(), strerror(errno));
  DEBUG("write");

  // close
  rc = socket_accept.Close();
  MUST_TRUE(rc == 0, strerror(errno));
  DEBUG("close");
}

TEST(Socket, ReadTimeout) {
  SKIP();
  // TODO
  // Finish it
  Address addr{"127.0.0.1", 8080};
  int rc = 0;
  const int max_connect_queue = 10;
  std::string msg = "hello world";

  Socket socket_recv;
  rc = socket_recv.Init();
  MUST_TRUE(rc == 0, socket_recv.err_msg());
  DEFER([&]() { socket_recv.Close(); });

  // set read max time
  rc = socket_recv.SetReadTimeout(1, 10);
  MUST_TRUE(rc == 0, socket_recv.err_msg());

  // bind
  rc = socket_recv.SetReuseAddr();
  MUST_TRUE(rc == 0, socket_recv.err_msg());
  rc = socket_recv.Bind(addr);
  MUST_TRUE(rc == 0, socket_recv.err_msg());

  // listen
  rc = socket_recv.Listen(max_connect_queue);
  MUST_TRUE(rc == 0, socket_recv.err_msg());

  // accept
  Address addr_cli;
  Socket socket_accept = socket_recv.Accept(addr_cli);
  MUST_TRUE(socket_accept.status() != Socket::kInit, "should timeout");
}
