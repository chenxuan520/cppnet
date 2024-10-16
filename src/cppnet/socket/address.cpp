#include "address.hpp"
#include <arpa/inet.h>

namespace cppnet {

Address::Address() {
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(0);
  addr_.sin_addr.s_addr = inet_addr("0.0.0.0");
}

Address::Address(const std::string &ip, uint16_t port) {
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);
  addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

void Address::GetIPAndPort(std::string &ip, uint16_t &port) {
  ip = inet_ntoa(addr_.sin_addr);
  port = ntohs(addr_.sin_port);
}

socklen_t *Address::GetAddrLen() {
  static socklen_t len = 0;
  len = sizeof(sockaddr_in);
  return &len;
}

int Address::GetPeerAddr(int fd, Address &addr) {
  socklen_t len = sizeof(sockaddr_in);
  return ::getpeername(fd, addr.GetSockAddr(), GetAddrLen());
}

} // namespace cppnet
