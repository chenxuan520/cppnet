#pragma once

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>

namespace cppapp {

class ProcessCtrl {
public:
  /*
   * @brief BackGround
   * @return pid_t
   * @note 1. Fork a child process.
   *      2. If the child process is created, the parent process will exit.
   *      3. The child process will continue to run in the background.
   */
  static int BackGround();
  /*
   * @brief Guard
   * @note 1. Fork a child process.
   *      2. If the child process is created, the parent process will wait for
   * the child process to exit.
   *      3. The child process will continue to run in the background.
   *      4. If the child process is killed, the parent process will fork a new
   * child process.
   */
  static void Guard();
  /*
   * @brief Kill
   * @param pid
   * @note 1. Send a signal to the process with the specified pid.
   */
  static void Kill(int pid, int signal = SIGINT);

private:
  static void EndGuard(int);

private:
  static int child_pid_;
};

} // namespace cppapp
