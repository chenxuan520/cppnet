#include "server/tcp_server.hpp"
#include "test.h"
#include "timer/timer.hpp"
#include <atomic>

using namespace cppnet;
using namespace std;

TEST(Timer, CreateTimer) {
#ifdef WIN32
  SKIP();
#endif

  atomic<int> count{0};
  Address addr{"127.0.0.1", 19981};
  TcpServer server{addr};
  auto rc = server.Init();
  MUST_TRUE(rc == 0, server.err_msg());

  TimerSocket timerfd(0, 1e5);
  MUST_TRUE(timerfd.status() != cppnet::Socket::kUninit, "create timerfd");
  DEFER_DEFAULT { timerfd.Close(); };
  rc = server.AddSoc(timerfd);

  // init callback
  auto event_func = [&](TcpServer::Event event, TcpServer &server, Socket fd) {
    if (event == TcpServer::kEventAccept) {
      // accept
      DEBUG(fd.fd() << " accept");
    } else if (event == TcpServer::kEventRead) {
      // get timer event
      uint64_t exp;
      TimerSocket timefd(fd);
      auto rc = timefd.Read(&exp, sizeof(uint64_t));
      MUST_TRUE(rc == sizeof(uint64_t), "read timerfd");
      count++;
    } else if (event == TcpServer::kEventLeave) {
      // leave, break loop
      DEBUG(fd.fd() << " leave " << event);
    } else {
      // dismiss
      DEBUG(fd.fd() << " dismiss " << event);
    }
  };
  server.Register(event_func);

  GO([&]() {
    usleep(1e4);
    server.Stop();
  });

  // start epoll loop
  rc = server.EventLoop();
  MUST_TRUE(rc == 0, server.err_msg());

  // check timer event
  // 1e7 = 10ms, 100ms = 1e8
  // 1s = 1e9
  DEBUG("timer event count: " << count);
#ifdef __linux__
  // tight 100us / 10ms window is sensitive to host load; under CI's
  // valgrind run it can drop to ~70. Use a loose envelope that still
  // catches gross regressions (timer not firing, or firing wildly).
  MUST_TRUE(count >= 20 && count <= 200,
            "timer event count wrong " + to_string(count));
#endif
}

TEST(Timer, ResetInPlace) {
#ifdef WIN32
  SKIP();
#endif

  // Verifies that Reset() updates the timer interval *in place* without
  // recreating the underlying fd. If a regression makes Reset() recreate
  // the fd, the new fd is not registered in epoll/kqueue and count_after
  // would stay zero.
  atomic<int> count_before{0};
  atomic<int> count_after{0};
  atomic<bool> reset_done{false};

  Address addr{"127.0.0.1", 19981};
  TcpServer server{addr};
  auto rc = server.Init();
  MUST_TRUE(rc == 0, server.err_msg());

  TimerSocket timerfd(0, 1e6); // 1ms
  MUST_TRUE(timerfd.status() != cppnet::Socket::kUninit, "create timerfd");
  DEFER_DEFAULT { timerfd.Close(); };

  int original_fd = timerfd.fd();

  rc = server.AddSoc(timerfd);
  MUST_TRUE(rc == 0, server.err_msg());

  server.Register([&](TcpServer::Event event, TcpServer &, Socket fd) {
    if (event == TcpServer::kEventRead) {
      uint64_t exp;
      TimerSocket(fd).Read(&exp, sizeof(uint64_t));
      if (reset_done.load()) {
        count_after.fetch_add(1);
      } else {
        count_before.fetch_add(1);
      }
    }
  });

  GO([&]() {
    usleep(2e4); // collect events with 1ms interval for ~20ms
    timerfd.Reset(0, 2e6); // slow down to 2ms
    MUST_TRUE(timerfd.fd() == original_fd,
              "Reset must not recreate fd: got " + to_string(timerfd.fd()) +
                  ", expected " + to_string(original_fd));
    reset_done.store(true);
    usleep(2e4); // observe events under the new interval for ~20ms
    server.Stop();
  });

  rc = server.EventLoop();
  MUST_TRUE(rc == 0, server.err_msg());

  DEBUG("ResetInPlace: before=" << count_before << " after=" << count_after);
  MUST_TRUE(count_before > 0,
            "no events before Reset, count=" + to_string(count_before));
  MUST_TRUE(count_after > 0,
            "no events after Reset (fd likely recreated and dropped from "
            "io_multiplexing), count=" +
                to_string(count_after));
}

TEST(Timer, CloseIdempotent) {
#ifdef WIN32
  SKIP();
#endif

  // Multiple Close() calls must not crash, double-free or leave dangling
  // resources. Memory issues here are caught by CI's valgrind run.
  TimerSocket timerfd(0, 1e6);
  MUST_TRUE(timerfd.status() != cppnet::Socket::kUninit, "create timerfd");

  timerfd.Close();
  timerfd.Close();
  timerfd.Close();
}
