#pragma once

#include "../socket/socket.hpp"
namespace cppnet {

class TimerSocket : public Socket {
public:
  TimerSocket(int fd) : Socket(fd) {}
  TimerSocket(Socket &soc) : Socket(soc) {}
  TimerSocket(int sec, int nsec) { Init(sec, nsec); }
  TimerSocket() = default;
  ~TimerSocket() = default;
  // forbiden copy
  TimerSocket(const TimerSocket &) = delete;
  TimerSocket &operator=(const TimerSocket &) = delete;
  /**
   * @brief: init timer.
   * @param sec: second
   * @param nsec: nano second, 1s = 1e9ns
   * @return: timerfd, -1 if failed.
   */
  int Init(int sec, int nsec);
  /**
   * @brief: Reset timer.
   * @param sec: second
   * @param nsec: nano second, 1s = 1e9ns
   * @return: 0 if success, -1 if failed.
   */
  int Reset(int sec, int nsec);

protected:
  inline int IORead(void *buf, size_t len, int flag = 0) override {
    return ::read(fd_, buf, len);
  }

  inline int IOWrite(const void *buf, size_t len, int flag = 0) override {
    return ::write(fd_, buf, len);
  }

#ifdef __APPLE__
public:
  int Close() override {
    Socket::Close();
    return Socket(send_fd_).Close();
  }

private:
  int send_fd_ = -1;
#endif
};

} // namespace cppnet
