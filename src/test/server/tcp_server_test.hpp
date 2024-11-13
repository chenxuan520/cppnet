#include "server/tcp_server.hpp"
#include "test.h"
#include <atomic>
#include <string>
#include <unistd.h>

using namespace cppnet;
using namespace std;

Address addr{"127.0.0.1", 8080};
TcpServer server{addr};

TEST(TcpServer, SigleClient) {
#ifdef _WIN32
  Address addr{"127.0.0.1",(uint16_t)(rand() % 1000 + 8000)};
  server.set_addr(addr);
#endif

  // init server
  auto rc = server.Init();
  MUST_TRUE(rc == 0, server.err_msg());
  DEFER([]() { server.Clean(); });

  // init msg
  string msg = "hello world";

  // init client
  Socket client;
  rc = client.Init();
  MUST_EQUAL(rc, 0);

  // init event function
  auto event_func = [&](TcpServer::Event event, TcpServer &server, Socket fd) {
    if (event == TcpServer::kEventAccept) {
      // accept
      DEBUG(fd.fd() << " accept");
    } else if (event == TcpServer::kEventRead) {
      // read
      string buf;
      auto ser_rc = fd.Read(buf, msg.size());
      DEBUG(fd.fd() << " read " << buf << " ");

    } else if (event == TcpServer::kEventLeave) {
      // leave, break loop
      DEBUG(fd.fd() << " leave " << event);
      server.Stop();
    } else {
      // dismiss
      DEBUG(fd.fd() << " dismiss " << event);
    }
  };
  server.Register(event_func);

  // thread to run client
  GO_JOIN([&]() {
    // connect
    auto cli_rc = client.Connect(addr);
    EXPECT_EQ(cli_rc, 0);
    DEBUG("client connect");

    // write
    cli_rc = client.Write(msg);
    EXPECT_EQ(cli_rc, msg.size());
    DEBUG("client write");

    // close
    cli_rc = client.Close();
    EXPECT_EQ(cli_rc, 0);
    DEBUG("client exit");
  });

  // run server
  server.EventLoop();
}

TEST(TcpServer, MultiClient) {
#ifdef _WIN32
  Address addr{"127.0.0.1",(uint16_t)(rand() % 1000 + 8000)};
  server.set_addr(addr);
#endif

  int loop_time = 1;
#ifdef __linux__
  loop_time = 1;
#endif

  for (int i = 0; i < loop_time; i += 1) {
#ifdef __linux__
    if (i % 2) {
      server.io_multiplexing()->set_trigger_type(
          IOMultiplexingBase::kEdgeTrigger);
    } else {
      server.io_multiplexing()->set_trigger_type(
          IOMultiplexingBase::kLevelTrigger);
    }
#endif
    // init server
    auto rc = server.Init();
    MUST_TRUE(rc == 0, server.err_msg());
    DEFER([]() { server.Clean(); });

    // init count
    const int test_num = 10;
    atomic<int> count{0};
    atomic<int> client_count{test_num};

    // init event function
    auto event_func = [&](TcpServer::Event event, TcpServer &server,
                          Socket fd) {
      if (event == TcpServer::kEventAccept) {
        Address addr_cli;
        fd.GetAddr(addr_cli);
        string ip;
        uint16_t port = 0;
        addr_cli.GetIPAndPort(ip, port);
        MUST_TRUE(port != 8080, "port need not equal 8080");
        DEBUG(fd.fd() << " accept " << ip << ":" << port);
        // accept
      } else if (event == TcpServer::kEventRead) {
        // read
        string buf;
        auto ser_rc = fd.Read(buf, 1);
        // DEBUG(fd.fd() << " read " << buf);
        // str2int
        int num = stoi(buf);
        // add count
        count += num;
        // send back
        ser_rc = fd.Write(to_string(num));
      } else if (event == TcpServer::kEventLeave) {
        // leave, break loop
        // DEBUG(fd.fd() << " leave ");
        client_count--;
        if (client_count == 0) {
          server.Stop();
        }
      } else {
        // dismiss
        DEBUG(fd.fd() << " dismiss " << event);
      }
    };
    server.Register(event_func);

    // thread to run client
    srand(time(nullptr));
    atomic<int> sum{0};
    cpptest::WaitGroup wait_group;
    for (int i = 0; i < test_num; i++) {
      wait_group.Add(GO_WAIT([&]() {
        int num = rand() % 10;
        sum += num;

        // init client
        Socket client;
        auto cli_rc = client.Init();
        MUST_EQUAL(cli_rc, 0);

        // connect
        cli_rc = client.Connect(addr);
        MUST_EQUAL(cli_rc, 0);
        // DEBUG("client connect ");

        // write
        cli_rc = client.Write(to_string(num));
        MUST_TRUE(cli_rc > 0, "write error");
        // DEBUG("client write " << to_string(num) << " ");

        // read back
        string buf;
        cli_rc = client.Read(buf, 1);
        MUST_EQUAL(cli_rc, buf.size());
        // DEBUG("client read " << buf << " " << i);

        // close
        usleep(10000);
        cli_rc = client.Close();
        EXPECT_EQ(cli_rc, 0);
        // DEBUG("client exit");
      }));
    }

    // run server
    rc = server.EventLoop();
    MUST_EQUAL(rc, 0);

    wait_group.Wait();

    // check
    DEBUG("count " << count << " sum " << sum);
    MUST_EQUAL(count, sum);
  }
}

TEST(TcpServer, MultiThread) {
#ifdef _WIN32
  Address addr{"127.0.0.1",(uint16_t)(rand() % 1000 + 8000)};
  server.set_addr(addr);
#endif
  // init msg
  string msg = "hello world";

  // init server
  server.set_mode(TcpServer::kMultiThread);
  auto rc = server.Init();
  MUST_TRUE(rc == 0, server.err_msg());
  DEFER([]() {
    server.Clean();
    server.set_mode(TcpServer::kIOMultiplexing);
  });

  // init event function
  auto event_func = [&](TcpServer::Event event, TcpServer &server, Socket fd) {
    if (event == TcpServer::kEventAccept) {
      Address addr_cli;
      auto rc = fd.GetAddr(addr_cli);
      MUST_EQUAL(rc, 0);
      string ip;
      uint16_t port = 0;
      addr_cli.GetIPAndPort(ip, port);
      MUST_TRUE(port != 8080, "port need not equal 8080");
      DEBUG(fd.fd() << " accept " << ip << ":" << port);

    } else if (event == TcpServer::kEventRead) {
      // read
      string buf;
      rc = fd.Read(buf, msg.size());
      MUST_TRUE(rc == msg.size(), "read error");
      DEBUG(fd.fd() << " read " << buf);

      // write
      rc = fd.Write(msg);
      MUST_TRUE(rc == msg.size(), "write error");
      DEBUG(fd.fd() << " write " << msg);

      // close
      rc = fd.Close();
      MUST_EQUAL(rc, 0);
      DEBUG(fd.fd() << " close");
      server.Stop();
    } else if (event == TcpServer::kEventLeave) {
      DEBUG(fd.fd() << " leave ");
    } else {
      DEBUG(fd.fd() << " error " << event);
      FATAL(server.err_msg());
    }
  };
  server.Register(event_func);

  // init client
  Socket client;
  rc = client.Init();
  MUST_EQUAL(rc, 0);

  // thread to run client
  GO_JOIN([&]() {
    // connect
    auto cli_rc = client.Connect(addr);
    MUST_EQUAL(cli_rc, 0);
    DEBUG("client connect");

    // write
    cli_rc = client.Write(msg);
    MUST_TRUE(cli_rc == msg.size(), strerror(errno));
    DEBUG("client write");

    // read back
    string buf;
    cli_rc = client.Read(buf, msg.size());
    MUST_TRUE(cli_rc == msg.size(), strerror(errno));
    DEBUG("client read " << buf);

    // close
    cli_rc = client.Close();
    MUST_EQUAL(cli_rc, 0);
    DEBUG("client exit");

    // wakeup server
    usleep(20);
    server.Stop();
    server.WakeUp();
  });

  // run server
  rc = server.EventLoop();
  MUST_TRUE(rc == 0, server.err_msg());
}

TEST(TcpServer, MixMode) {
#ifdef _WIN32
  Address addr{"127.0.0.1",(uint16_t)(rand() % 1000 + 8000)};
  server.set_addr(addr);
#endif
  // SKIP();
  int loop_time = 1;

  for (int i = 0; i < loop_time; i += 1) {
    // init server
    server.set_mode(cppnet::TcpServer::kMixed);
    auto rc = server.Init();
    MUST_TRUE(rc == 0, server.err_msg());
    DEFER([]() { server.Clean(); });

    // init count
    const int test_num = 2;
    atomic<int> count{0};
    atomic<int> client_count{test_num};

    // init event function
    auto event_func = [&](TcpServer::Event event, TcpServer &server,
                          Socket fd) {
      if (event == TcpServer::kEventAccept) {
        Address addr_cli;
        fd.GetAddr(addr_cli);
        string ip;
        uint16_t port = 0;
        addr_cli.GetIPAndPort(ip, port);
        MUST_TRUE(port != 8080, "port need not equal 8080");
        // DEBUG(fd.fd() << " accept " << ip << ":" << port);
        // accept
      } else if (event == TcpServer::kEventRead) {
        // read
        string buf;
        auto ser_rc = fd.Read(buf, 1);
        // DEBUG(fd.fd() << " read " << buf);
        if (buf.size() == 0) {
          return;
        }
        // str2int
        int num = stoi(buf);
        // add count
        count += num;
        // send back
        ser_rc = fd.Write(to_string(num));
      } else if (event == TcpServer::kEventLeave) {
        // leave, break loop
        client_count--;
        if (client_count == 0) {
          DEBUG("server stop");
          server.Stop();
        }
        DEBUG(fd.fd() << " leave " << client_count);
      } else {
        // dismiss
        DEBUG(fd.fd() << " dismiss " << event);
      }
    };
    server.Register(event_func);

    // thread to run client
    srand(time(nullptr));
    atomic<int> sum{0};
    cpptest::WaitGroup wait_group;
    for (int i = 0; i < test_num; i++) {
      wait_group.Add(GO_WAIT([&]() {
        int num = rand() % 10;
        sum += num;

        // init client
        Socket client;
        auto cli_rc = client.Init();
        MUST_EQUAL(cli_rc, 0);

        // connect
        cli_rc = client.Connect(addr);
        MUST_EQUAL(cli_rc, 0);
        DEBUG("client connect ");

        // write
        cli_rc = client.Write(to_string(num));
        MUST_TRUE(cli_rc > 0, "write error");
        DEBUG("client write " << to_string(num) << " ");

        // read back
        string buf;
        cli_rc = client.Read(buf, 1);
        MUST_EQUAL(cli_rc, buf.size());
        DEBUG("client read " << buf << " " << i);

        // close
        usleep(1000);
        cli_rc = client.Close();
        EXPECT_EQ(cli_rc, 0);
        DEBUG("client exit");
      }));
    }

    // run server
    rc = server.EventLoop();
    MUST_EQUAL(rc, 0);

    wait_group.Wait();

    // check
    DEBUG("count " << count << " sum " << sum);
    MUST_EQUAL(count, sum);
  }
}
