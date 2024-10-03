#include "address.hpp"
#include <arpa/inet.h>

namespace cppnet {

Address::Address(const std::string &ip, uint16_t port) {
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);
  addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

void Address::GetIPAndPort(std::string &ip, uint16_t &port) {
  ip = inet_ntoa(addr_.sin_addr);
  port = ntohs(addr_.sin_port);
}

} // namespace cppnet
