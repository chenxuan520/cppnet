#pragma once

#include <csignal>
#include <cstdlib>
#include <functional>
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
  static void BackGround();
  /*
   * @brief Guard
   * @note 1. Fork a child process.
   *      2. If the child process is created, the parent process will wait for
   * the child process to exit.
   *      3. The child process will continue to run in the background.
   *      4. If the child process is killed, the parent process will fork a new
   * child process.
   *      5. If the exit_code_handler is not null, the parent process will call,
   * return code to control if continue to fork a new child process.
   *      6. The parent process will sleep for wait_seconds before forking a new
   * child
   */
  using ExitCodeHandler =
      std::function<bool(int exit_code, bool is_normal_exit)>;
  static void Guard(const ExitCodeHandler &exit_code_handler = nullptr,
                    int wait_seconds = 5);
  /*
   * @brief Kill
   * @param pid
   * @note 1. Send a signal to the process with the specified pid.
   */
  static int Kill(int pid, int signal = SIGINT);

private:
  static void EndGuard(int);

private:
  static int child_pid_;
};

} // namespace cppapp
