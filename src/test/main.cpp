#include "server/tcp_server_test.hpp"
#include "socket/address_test.hpp"
#include "socket/socket_test.hpp"
#include "test.h"
#include "timer/timer_test.hpp"
#include "utils/host_test.hpp"
#include "utils/threadpoll_test.hpp"

INIT(Main) {
  GO([&]() {
    // max run time
    sleep(10);
    _TESTSTDERR_("Test Task Cost Too Much Time, Killed");
    exit(-1);
  });
}

ARGC_FUNC {
  if (argc == 2) {
    REGEX_FILT_TEST(argv[1]);
  }
}
