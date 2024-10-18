#include "host.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

namespace cppnet {

std::string Host::GetLocalName() {
  char name[512] = {0};
  gethostname(name, sizeof(name));
  return name;
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
