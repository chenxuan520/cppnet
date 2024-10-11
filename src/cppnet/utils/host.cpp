#include "host.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

namespace cppnet {

std::string Host::GetLocalName() {
  char ip[512] = {0};
  gethostname(ip, sizeof(ip));
  return ip;
}

std::string Host::GetLocalIP() {
  char ip[512] = {0};
  gethostname(ip, sizeof(ip));
  struct hostent *host = gethostbyname(ip);
  if (host == nullptr) {
    return "";
  }
  return inet_ntoa(*(struct in_addr *)host->h_addr_list[0]);
}

} // namespace cppnet
