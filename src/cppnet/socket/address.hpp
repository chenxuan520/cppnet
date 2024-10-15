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
  /**
   * @brief: Get ip and port from addr_.
   */
  void GetIPAndPort(std::string &ip, uint16_t &port);

public:
  sockaddr_in *GetAddr() { return &addr_; }
  sockaddr *GetSockAddr() { return reinterpret_cast<sockaddr *>(&addr_); }
  static socklen_t *GetAddrLen();

private:
  sockaddr_in addr_{0, 0, {0}};
};

} // namespace cppnet
