#include "kqueue.hpp"
#include "utils/const.hpp"

#ifdef __APPLE__

#include <sys/event.h>

namespace cppnet {

KQueue::KQueue() : IOMultiplexingBase() {}

KQueue::~KQueue() { Close(); }

int KQueue::Init() {
  kq_fd_ = kqueue();
  if (kq_fd_.status() == Socket::kInit) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    return kSysErr;
  }
  return kSuccess;
}

int KQueue::MonitorSoc(const Socket &fd) {
  struct kevent ke;
  EV_SET(&ke, fd.fd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr);
  if (kevent(kq_fd_.fd(), &ke, 1, nullptr, 0, nullptr) == -1) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    return kSysErr;
  }
  return kSuccess;
}

int KQueue::RemoveSoc(const Socket &fd) {
  struct kevent ke;
  EV_SET(&ke, fd.fd(), EVFILT_READ, EV_DELETE, 0, 0, nullptr);
  if (kevent(kq_fd_.fd(), &ke, 1, nullptr, 0, nullptr) == -1) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    return kSysErr;
  }
  return kSuccess;
}

int KQueue::Loop(NotifyCallBack callback) {
  while (loop_flag_) {
    struct kevent evs[1024];
    int nfds = kevent(kq_fd_.fd(), nullptr, 0, evs, 1024, nullptr);
    if (nfds < 0) {
      if (errno == EINTR) {
      }
      err_msg_ = "[syserr]:" + std::string(strerror(errno));
      return kSysErr;
    }
    for (int i = 0; i < nfds; ++i) {
      if (evs[i].filter == EVFILT_READ) {
        if (callback != nullptr) {
          callback(*this, evs[i].ident);
        }
      }
    }
  }
  return 0;
}

void KQueue::Close() { kq_fd_.Close(); }

} // namespace cppnet

#endif
