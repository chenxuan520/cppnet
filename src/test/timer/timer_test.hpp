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
  Address addr{"127.0.0.1", 8080};
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
  MUST_TRUE(count >= 90 && count <= 110,
            "timer event count wrong " + to_string(count));
#endif
}
