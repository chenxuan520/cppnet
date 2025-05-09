#include "process_ctrl.hpp"
#include <cstdio>

namespace cppapp {

void ProcessCtrl::BackGround() {
  int pid = 0;
#ifndef _WIN32
  if ((pid = fork()) != 0) {
    printf("process pid=%d\n", pid);
    exit(0);
  }
#endif
}

void ProcessCtrl::Guard(const ExitCodeHandler &exit_code_handler,
                        int wait_seconds) {
#ifndef _WIN32
  signal(SIGINT, EndGuard);
  signal(SIGQUIT, EndGuard);
  signal(SIGTERM, EndGuard);
  while (1) {
    int pid = fork();
    int status = 0;
    if (pid != 0) {
      child_pid_ = pid;
      waitpid(pid, &status, 0);
      if (exit_code_handler != nullptr) {
        int exit_status = WEXITSTATUS(status);
        auto is_continue = exit_code_handler(exit_status, WIFEXITED(status));
        if (!is_continue) {
          break;
        }
      }
      sleep(wait_seconds);
    } else {
      break;
    }
  }
#endif
}

void ProcessCtrl::EndGuard(int) {
#ifndef _WIN32
  if (ProcessCtrl::child_pid_ != 0)
    kill(ProcessCtrl::child_pid_, 2);
  exit(0);
#endif
}

int ProcessCtrl::Kill(int pid, int signal) {
#ifndef _WIN32
  return kill(pid, signal);
#else
  return -1;
#endif
}

int ProcessCtrl::child_pid_ = 0;

} // namespace cppapp
