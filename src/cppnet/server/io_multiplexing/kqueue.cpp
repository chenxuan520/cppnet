#ifdef __APPLE__

#include "kqueue.hpp"
#include "../../utils/const.hpp"
#include <sys/event.h>

namespace cppnet {

KQueue::KQueue() : IOMultiplexingBase() {}

KQueue::~KQueue() { Close(); }

int KQueue::Init() {
  kq_fd_ = kqueue();
  if (kq_fd_.status() != Socket::kInit) {
    err_msg_ = "[syserr]: " + Socket::err_msg();
    return kSysErr;
  }
  return kSuccess;
}

int KQueue::MonitorSoc(const Socket &fd) {
  struct kevent ke;
  EV_SET(&ke, fd.fd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr);
  if (kevent(kq_fd_.fd(), &ke, 1, nullptr, 0, nullptr) == -1) {
    err_msg_ = "[syserr]:" + Socket::err_msg();
    return kSysErr;
  }
  return kSuccess;
}

int KQueue::RemoveSoc(const Socket &fd) {
  struct kevent ke;
  EV_SET(&ke, fd.fd(), EVFILT_READ, EV_DELETE, 0, 0, nullptr);
  if (kevent(kq_fd_.fd(), &ke, 1, nullptr, 0, nullptr) == -1) {
    err_msg_ = "[syserr]:" + Socket::err_msg();
    return kSysErr;
  }
  return kSuccess;
}

int KQueue::Loop(NotifyCallBack callback) {
  if (callback == nullptr) {
    err_msg_ = "[logicerr]:" + std::string("callback is nullptr");
    return kLogicErr;
  }

  while (loop_flag_) {
    struct kevent evs[max_event_num_];
    timespec *ts = nullptr;
    if (wait_timeout_ > 0) {
      ts = new timespec;
      ts->tv_sec = wait_timeout_ / 1000;
      ts->tv_nsec = (wait_timeout_ % 1000) * 1000000;
    }

    int nfds = kevent(kq_fd_.fd(), nullptr, 0, evs, max_event_num_, ts);

    if (ts != nullptr) {
      delete ts;
    }

    if (nfds < 0) {
      if (errno == EINTR) {
      }
      err_msg_ = "[syserr]:" + std::string(strerror(errno));
      return kSysErr;
    }
    for (int i = 0; i < nfds; ++i) {
      if (evs[i].flags & EV_EOF) {
        callback(*this, evs[i].ident, kIOEventLeave);
      } else if (evs[i].flags & EV_ERROR) {
        callback(*this, evs[i].ident, kIOEventError);
      } else if (evs[i].filter == EVFILT_READ) {
        callback(*this, evs[i].ident, kIOEventRead);
      }
    }
  }
  return 0;
}

void KQueue::Close() { kq_fd_.Close(); }

} // namespace cppnet

#endif
