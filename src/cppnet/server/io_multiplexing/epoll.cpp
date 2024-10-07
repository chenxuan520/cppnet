#include "epoll.hpp"
#include "utils/const.hpp"
#include <cstring>
#include <sys/epoll.h>

namespace cppnet {

Epoll::TriggerType Epoll::trigger_type_ = Epoll::TriggerType::kEdgeTrigger;

int Epoll::Init() {
  epoll_fd_ = CreateEpoll();
  if (epoll_fd_.status() == Socket::kInit) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    return -1;
  }
  return 0;
}

int Epoll::MonitorSoc(const Socket &fd) {
  return UpdateEpollEvents(epoll_fd_.fd(), EPOLL_CTL_ADD, fd.fd(),
                           EPOLLIN | EPOLLET | EPOLLRDHUP);
}

int Epoll::RemoveSoc(const Socket &fd) {
  return UpdateEpollEvents(epoll_fd_.fd(), EPOLL_CTL_DEL, fd.fd(), 0);
}

void Epoll::Close() { epoll_fd_.Close(); }

int Epoll::Loop(NotifyCallBack callback) {
  while (loop_flag_) {
    struct epoll_event evs[max_event_num_];
    int nfds = epoll_wait(epoll_fd_.fd(), evs, max_event_num_, -1);
    if (nfds < 0) {
      if (errno == EINTR) {
        continue;
      }
      err_msg_ = "[syserr]:" + std::string(strerror(errno));
      return -1;
    }
    for (int i = 0; i < nfds; ++i) {
      if (callback != nullptr) {
        callback(*this, evs[i].data.fd);
      }
    }
  }
  return 0;
}

Socket Epoll::CreateEpoll() { return epoll_create1(0); }

int Epoll::UpdateEpollEvents(int epfd, int op, int fd, int event) {
  struct epoll_event ev;
  ev.events = event;
  ev.data.fd = fd;
  return epoll_ctl(epfd, op, fd, &ev);
}

} // namespace cppnet
