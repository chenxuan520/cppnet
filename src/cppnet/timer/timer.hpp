#pragma once

namespace cppnet {

class Timer {
public:
  /**
   * @brief: Create timer.
   * @param sec: second
   * @param nsec: nano second
   * @return: timerfd, -1 if failed.
   */
  static int CreateTimer(int sec, int nsec);
  /**
   * @brief: Reset timer.
   * @param timerfd: timerfd
   * @param sec: second
   * @param nsec: nano second, 1s = 1e9ns
   * @return: 0 if success, -1 if failed.
   */
  static int ResetTimer(int timerfd, int sec, int nsec);
  /**
   * @brief: Remove timer.
   * @param timerfd: timerfd
   * @return: 0 if success, -1 if failed.
   */
  static int RemoveTimer(int timerfd);
};

} // namespace cppnet
