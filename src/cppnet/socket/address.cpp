#include "address.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>

namespace cppnet {

Address::Address() {
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(0);
  addr_.sin_addr.s_addr = inet_addr("0.0.0.0");
}

Address::Address(const std::string &ip, uint16_t port) { Init(ip, port); }

int Address::Init(const std::string &ip, uint16_t port) {
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);
  addr_.sin_addr.s_addr = inet_addr(ip.c_str());
  return 0;
}

int Address::InitWithDomain(const std::string &domain, uint16_t port) {
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);

  hostent *phost = gethostbyname(domain.c_str());
  if (phost == nullptr) {
    return -1;
  }
  in_addr addr;
  char *p = phost->h_addr_list[0];
  memcpy(&addr.s_addr, p, phost->h_length);

  addr_.sin_addr.s_addr = inet_addr(inet_ntoa(addr));
  return 0;
}

void Address::GetIPAndPort(std::string &ip, uint16_t &port) const {
  ip = inet_ntoa(addr_.sin_addr);
  port = ntohs(addr_.sin_port);
}

std::string Address::ToString() const {
  std::string ip;
  uint16_t port;
  GetIPAndPort(ip, port);
  return ip + ":" + std::to_string(port);
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
