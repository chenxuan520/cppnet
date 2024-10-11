#pragma once

#include "socket/address.hpp"
#include "test.h"

using namespace cppnet;

TEST(Address, AddressTurn) {
  Address addr("127.0.0.1", 80);
  std::string ip;
  uint16_t port;
  addr.GetIPAndPort(ip, port);
  MUST_TRUE(ip == "127.0.0.1", ip + " is not 127.0.0.1");
  MUST_TRUE(port == 80, std::to_string(port) + " is not 80");
}
