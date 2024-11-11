
#ifdef _WIN32
#include "select.hpp"
#include <winsock2.h>

namespace cppnet {

Select::Select() : IOMultiplexingBase() {
  WSADATA wsa; // web server api data
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    printf("wsadata wrong\n");
    exit(0);
  }
}
Select::~Select() { WSACleanup(); }

int Select::Init() {
  FD_ZERO(&socket_set_);
  return 0;
}

int Select::MonitorSoc(const Socket &fd) {
  FD_SET(fd.fd(), &socket_set_);
  return 0;
}

int Select::RemoveSoc(const Socket &fd) {
  FD_CLR(fd.fd(), &socket_set_);
  return 0;
}

void Select::Close() { FD_ZERO(&socket_set_); }

int Select::Loop(NotifyCallBack callback) {
  int ret = 0;
  while (true) {
    fd_set event_set = socket_set_;
    ret = select(0, &event_set, nullptr, nullptr, nullptr);
    if (ret == -1) {
      if (errno == EINTR) {
        continue;
      }
      return ret;
    }
  }
}

} // namespace cppnet

#endif
