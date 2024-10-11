#pragma once

#include "socket/socket.hpp"
#include "test.h"

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
