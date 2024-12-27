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
  if (send_fd_ > 0) {
    Close();
  }

  int pipe_fd[2] = {-1, -1};
  if (pipe(pipe_fd) < 0) {
    return kSysErr;
  }

  fd_ = pipe_fd[0];
  send_fd_ = pipe_fd[1];
  status_ = kInit;

  auto timer_func = [=](int send_fd, int sec, int nsec) {
    uint64_t exp;
    TimerSocket soc(send_fd);
    while (true) {
      std::this_thread::sleep_for(
          std::chrono::nanoseconds((long long)(sec * 1e9) + nsec));
      auto rc = soc.Write(&exp, sizeof(uint64_t));
      if (rc <= 0) {
        // close socket
        break;
      }
    }
  };

  thread_ = std::move(std::thread(timer_func, send_fd_, sec, nsec));

  return kSuccess;
#else
  return kNotSupport;
#endif
}

#ifdef __APPLE__
int TimerSocket::Close() {
  Socket::Close();
  Socket(send_fd_).Close();
  thread_.join();
  send_fd_ = -1;
  return 0;
}
#endif

} // namespace cppnet
