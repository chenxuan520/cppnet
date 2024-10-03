#pragma once

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
namespace cppnet {

class Address {
public:
  Address() = default;
  Address(const std::string &ip, uint16_t port);
  Address(sockaddr_in addr) : addr_(addr){};
  void GetIPAndPort(std::string &ip, uint16_t &port);

public:
  sockaddr_in *GetAddr() { return &addr_; }
  sockaddr *GetSockAddr() { return reinterpret_cast<sockaddr *>(&addr_); }

private:
  sockaddr_in addr_;
};

} // namespace cppnet
