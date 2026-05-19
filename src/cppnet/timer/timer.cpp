#include "timer.hpp"
#include "../utils/const.hpp"

#ifdef __linux__
#include <sys/timerfd.h>
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
  return Reset(sec, nsec);
#else
  return kNotSupport;
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

#elif __APPLE__
  uint64_t interval_ns = (uint64_t)sec * NSEC_PER_SEC + (uint64_t)nsec;

  // fast path: timer already running, just update interval in-place
  if (timer_ != nullptr) {
    dispatch_source_set_timer(timer_, DISPATCH_TIME_NOW, interval_ns, 0);
    return kSuccess;
  }

  // first time: create pipe (read end exposed as Socket fd) + GCD timer
  int pipe_fd[2] = {-1, -1};
  if (pipe(pipe_fd) < 0) {
    return kSysErr;
  }
  fd_ = pipe_fd[0];
  send_fd_ = pipe_fd[1];

  timer_ = dispatch_source_create(
      DISPATCH_SOURCE_TYPE_TIMER, 0, 0,
      dispatch_get_global_queue(QOS_CLASS_USER_INITIATED, 0));
  if (timer_ == nullptr) {
    ::close(fd_);
    ::close(send_fd_);
    fd_ = -1;
    send_fd_ = -1;
    return kSysErr;
  }

  // capture by value: send_fd_ may be reset to -1 in Close()
  int send_fd = send_fd_;
  dispatch_source_set_event_handler(timer_, ^{
    uint64_t exp = 1;
    ::write(send_fd, &exp, sizeof(uint64_t));
  });

  // leeway = 0 disables timer coalescing for higher precision
  dispatch_source_set_timer(timer_, DISPATCH_TIME_NOW, interval_ns, 0);
  dispatch_resume(timer_);
  status_ = kInit;
  return kSuccess;
#else
  return kNotSupport;
#endif
}

#ifdef __APPLE__
int TimerSocket::Close() {
  if (timer_ != nullptr) {
    // dispatch_source_cancel is async; use semaphore to wait for the cancel
    // handler so the event handler can no longer write to send_fd_ after
    // Close() returns
    dispatch_semaphore_t sem = dispatch_semaphore_create(0);
    dispatch_source_set_cancel_handler(timer_, ^{
      dispatch_semaphore_signal(sem);
    });
    dispatch_source_cancel(timer_);
    dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);
#if !OS_OBJECT_USE_OBJC
    dispatch_release(timer_);
    dispatch_release(sem);
#endif
    timer_ = nullptr;
  }

  if (send_fd_ >= 0) {
    ::close(send_fd_);
    send_fd_ = -1;
  }
  if (status_ == kInit) {
    Socket::Close();
  }
  return kSuccess;
}
#endif

} // namespace cppnet
