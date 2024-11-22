#pragma once

#include <string>

#ifndef _WIN32
#include <netinet/in.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
using socklen_t = int;
#endif

namespace cppnet {

class Address {
public:
  Address();
  Address(const std::string &ip, uint16_t port);
  Address(sockaddr_in addr) : addr_(addr) {};
  /**
   * @brief: init with ip and port
   */
  int Init(const std::string &ip, uint16_t port);
  /**
   * @brief: init with domain and port
   */
  int InitWithDomain(const std::string &domain, uint16_t port);
  /**
   * @brief: Get ip and port from addr_.
   */
  void GetIPAndPort(std::string &ip, uint16_t &port) const;
  /**
   * @brief: Get ip from addr_.
   */
  std::string GetIP() const;
  /**
   * @brief: Get port from addr_.
   */
  uint16_t GetPort() const;
  /**
   * @brief: Make to string
   */
  std::string ToString() const;

public:
  sockaddr_in *GetAddr() { return &addr_; }
  sockaddr *GetSockAddr() { return reinterpret_cast<sockaddr *>(&addr_); }
  static socklen_t *GetAddrLen();
  /**
   * @brief: Get peer address.
   * @param fd: socket fd.
   * @param addr: peer address.
   * @return: 0 if success, -1 if failed.
   */
  static int GetPeerAddr(int fd, Address &addr);

private:
  sockaddr_in addr_;
};

} // namespace cppnet
