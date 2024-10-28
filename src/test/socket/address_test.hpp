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

TEST(Address, AddressUninit) {
  Address addr;
  std::string ip;
  uint16_t port;
  addr.GetIPAndPort(ip, port);
  DEBUG("ip: " + ip + " port: " + std::to_string(port));
  MUST_TRUE(ip == "0.0.0.0", ip + " is not 0.0.0.0");
  MUST_TRUE(port == 0, std::to_string(port) + " is not 0");
}

TEST(Address, AddressDomain) {
  Address addr;
  auto rc = addr.InitWithDomain("localhost", 80);
  MUST_TRUE(rc == 0, "init with domain error");
  std::string ip;
  uint16_t port;
  addr.GetIPAndPort(ip, port);
  DEBUG("ip: " + ip + " port: " + std::to_string(port));
  MUST_EQUAL(ip, "127.0.0.1");
}
