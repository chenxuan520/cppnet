#ifdef __linux__

#include "epoll.hpp"
#include "../../utils/const.hpp"
#include <cstring>
#include <sys/epoll.h>

namespace cppnet {

int Epoll::Init() {
  epoll_fd_ = CreateEpoll();
  if (epoll_fd_.status() != Socket::kInit) {
    err_msg_ = "[syserr]:" + Socket::err_msg();
    return kSysErr;
  }
  return kSuccess;
}

int Epoll::MonitorSoc(const Socket &fd) {
  if (trigger_type_ == kEdgeTrigger) {
    auto rc = fd.SetNoBlock();
    if (rc < 0) {
      err_msg_ = "[syserr]:" + Socket::err_msg();
      return kSysErr;
    }
    return UpdateEpollEvents(epoll_fd_.fd(), EPOLL_CTL_ADD, fd.fd(),
                             EPOLLIN | EPOLLET | EPOLLRDHUP);
  } else {
    return UpdateEpollEvents(epoll_fd_.fd(), EPOLL_CTL_ADD, fd.fd(),
                             EPOLLIN | EPOLLRDHUP);
  }
  return kSuccess;
}

int Epoll::RemoveSoc(const Socket &fd) {
  return UpdateEpollEvents(epoll_fd_.fd(), EPOLL_CTL_DEL, fd.fd(), 0);
}

void Epoll::Close() { epoll_fd_.Close(); }

int Epoll::Loop(NotifyCallBack callback) {
  if (callback == nullptr) {
    err_msg_ = "[logicerr]:" + std::string("callback is nullptr");
    return kLogicErr;
  }

  while (loop_flag_) {
    struct epoll_event evs[max_event_num_];
    int nfds = epoll_wait(epoll_fd_.fd(), evs, max_event_num_, wait_timeout_);
    if (nfds < 0) {
      if (errno == EINTR) {
        continue;
      }
      err_msg_ = "[syserr]:" + Socket::err_msg();
      return kSysErr;
    }
    for (int i = 0; i < nfds; ++i) {
      if (evs[i].events & EPOLLRDHUP) {
        callback(*this, evs[i].data.fd, kIOEventLeave);

      } else if (evs[i].events & EPOLLERR) {
        callback(*this, evs[i].data.fd, kIOEventError);

      } else if (evs[i].events & EPOLLIN) {
        callback(*this, evs[i].data.fd, kIOEventRead);
      }
    }
  }
  return kSuccess;
}

Socket Epoll::CreateEpoll() { return epoll_create1(0); }

int Epoll::UpdateEpollEvents(int epfd, int op, int fd, int event) {
  struct epoll_event ev;
  ev.events = event;
  ev.data.fd = fd;
  return epoll_ctl(epfd, op, fd, &ev);
}

} // namespace cppnet

#endif
