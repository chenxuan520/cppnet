#include "process_ctrl.hpp"

namespace cppapp {

int ProcessCtrl::BackGround() {
  int pid = 0;
#ifndef _WIN32
  if ((pid = fork()) != 0) {
    printf("process pid=%d\n", pid);
    exit(0);
  }
#endif
  return pid;
}

void ProcessCtrl::Guard() {
  const int wait_seconds = 5;
#ifndef _WIN32
  signal(SIGINT, EndGuard);
  signal(SIGQUIT, EndGuard);
  signal(SIGTERM, EndGuard);
  while (1) {
    int pid = fork();
    if (pid != 0) {
      child_pid_ = pid;
      waitpid(pid, NULL, 0);
      sleep(wait_seconds);
    } else
      break;
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

void ProcessCtrl::Kill(int pid, int signal) {
#ifndef _WIN32
  kill(pid, signal);
#endif
}

int ProcessCtrl::child_pid_ = 0;

} // namespace cppapp
