
#ifdef _WIN32
#include "select.hpp"
#include <winsock2.h>

namespace cppnet {

Select::Select() : IOMultiplexingBase() {
}
Select::~Select() {}

int Select::Init() {
  FD_ZERO(&socket_set_);
  return 0;
}

int Select::MonitorSoc(const Socket &fd) {
  FD_SET(fd.fd(), &socket_set_);
  if(fd.fd()>max_fd_){
    max_fd_=fd.fd();
  }
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
    ret = select(max_fd_+1, &event_set, nullptr, nullptr, nullptr);
    if (ret == -1) {
      if (errno == EINTR) {
        continue;
      }
      return ret;
    }
    for (int i = 0; i < max_fd_+1; ++i) {
      if (FD_ISSET(i, &event_set)) {
        Socket socket(i);
        // check if exit or read
        std::string tmp_buf;
        auto rc=socket.ReadPeek(tmp_buf, 1);
        if(rc==0){
          callback(*this, socket,kIOEventLeave);
        }else if(rc>0){
          callback(*this, socket,kIOEventRead);
        }else{
          callback(*this, socket,kIOEventError);
        }
      }
    }
  }
}

} // namespace cppnet

#endif
