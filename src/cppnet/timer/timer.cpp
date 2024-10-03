
#include "timer.hpp"

#include <sys/timerfd.h>
#include <unistd.h>

namespace cppnet {

int Timer::CreateTimer(int sec, int nsec) {
  int timerfd = timerfd_create(CLOCK_REALTIME, 0);
  if (timerfd <= 0) {
    return -1;
  }

  ResetTimer(timerfd, sec, nsec);
  return timerfd;
}

int Timer::ResetTimer(int timerfd, int sec, int nsec) {
  struct itimerspec ts;
  ts.it_value.tv_sec = sec;
  ts.it_value.tv_nsec = nsec;
  ts.it_interval.tv_sec = sec;
  ts.it_interval.tv_nsec = nsec;

  timerfd_settime(timerfd, 0, &ts, NULL);
  return 0;
}

int Timer::RemoveTimer(int timerfd) { return ::close(timerfd); }

} // namespace cppnet
