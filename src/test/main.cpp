#include "server/tcp_server_test.hpp"
#include "test.h"

ARGC_FUNC {
  if (argc == 2) {
    REGEX_FILT_TEST(argv[1]);
  }
}
