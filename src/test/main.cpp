#include "server/tcp_server_test.hpp"
#include "test.h"
#include "timer/timer_test.hpp"

ARGC_FUNC {
  if (argc == 2) {
    REGEX_FILT_TEST(argv[1]);
  }
}
