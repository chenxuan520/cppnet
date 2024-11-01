
#include "timer.hpp"
#include "../utils/const.hpp"

#ifdef __linux__
#include <sys/timerfd.h>
#elif __APPLE__
#include <dispatch/dispatch.h>
#endif

#include <unistd.h>

namespace cppnet {

int TimerSocket::Init(int sec, int nsec) {
  // only linux support this feature
#ifdef __linux__
  fd_ = timerfd_create(CLOCK_REALTIME, 0);
  if (fd_ <= 0) {
    return kSysErr;
  }

  Reset(sec, nsec);
  status_ = kInit;
  return kSuccess;
#elif __APPLE__
  return kNotSupport;
#else
  return kUnSupport;
#endif
}

int TimerSocket::Reset(int sec, int nsec) {
#ifdef __linux__
  struct itimerspec ts;
  ts.it_value.tv_sec = sec;
  ts.it_value.tv_nsec = nsec;
  ts.it_interval.tv_sec = sec;
  ts.it_interval.tv_nsec = nsec;

  timerfd_settime(fd_, 0, &ts, NULL);
  return kSuccess;
#else
  return kNotSupport;
#endif
}
} // namespace cppnet
