#include "http/req/http_req_test.hpp"
#include "http/resp/http_resp_test.hpp"
#include "server/tcp_server_test.hpp"
#include "socket/address_test.hpp"
#include "socket/socket_test.hpp"
#include "ssl/ssl_context_test.hpp"
#include "test.h"
#include "timer/timer_test.hpp"
#include "utils/file_test.hpp"
#include "utils/host_test.hpp"
#include "utils/string_test.hpp"
#include "utils/threadpoll_test.hpp"
#include "utils/trie_test.hpp"

#include <signal.h>

INIT(Main) {
  // set for signal
  signal(SIGPIPE, SIG_IGN);
  GO([&]() {
    // max run time
    sleep(10);
    WARNING("Test Task Cost Too Much Time, Killed");
    exit(-1);
  });
}

ARGC_FUNC {
  if (argc == 2) {
    REGEX_FILT_TEST(argv[1]);
  }
}
