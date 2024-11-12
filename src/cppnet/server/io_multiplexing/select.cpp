
#ifdef _WIN32
#include "select.hpp"
#include <winsock2.h>
#include <iostream>

namespace cppnet {

Select::Select() : IOMultiplexingBase() {
}
Select::~Select() {}

int Select::Init() {
  FD_ZERO(&monitor_set_);
  return 0;
}

int Select::MonitorSoc(const Socket &fd) {
  FD_SET(fd.fd(), &monitor_set_);
  if(fd.fd()>max_fd_){
    max_fd_=fd.fd();
  }
  return 0;
}

int Select::RemoveSoc(const Socket &fd) {
  FD_CLR(fd.fd(), &monitor_set_);
  return 0;
}

void Select::Close() { FD_ZERO(&monitor_set_); }

int Select::Loop(NotifyCallBack callback) {
  int ret = 0;
  while (loop_flag_) {
    fd_set event_set = monitor_set_;
    std::cout<<max_fd_+1<<" "<<event_set.fd_count<<std::endl;
    ret = select(0, &event_set, nullptr, nullptr, nullptr);
    std::cout<<max_fd_+1<<ret<<std::endl;
    if (ret < 0) {
      return ret;
    }
    for (int i = 0; i < monitor_set_.fd_count; ++i) {
      if (FD_ISSET(monitor_set_.fd_array[i], &event_set)) {
        Socket socket(monitor_set_.fd_array[i]);
        std::string tmp_buf(1,0);
        auto rc=socket.ReadPeek(tmp_buf, 1);
        if(rc==0){
          callback(*this, socket,kIOEventLeave);
        }else if(rc>0){
          callback(*this, socket,kIOEventRead);
        }else{
          callback(*this, socket,kIOEventError);
        }
        // callback(*this, monitor_set_.fd_array[i],kIOEventRead);
      }
    }
  }
  return 0;
}

} // namespace cppnet

#endif
