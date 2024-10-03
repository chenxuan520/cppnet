#include "server/tcp_server.hpp"
#include "test.h"
#include <atomic>
#include <string>
#include <unistd.h>
#include <vector>

using namespace cppnet;
using namespace std;

Address addr{"127.0.0.1", 8080};
TcpServer server{addr};

INIT(TcpServer) {
  GO([&]() {
    // max run time
    sleep(10);
    exit(-1);
  });
}

TEST(TcpServer, SigleClient) {
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
  server.EpollLoop();
}

TEST(TcpServer, MultiClient) {

  // init server
  auto rc = server.Init();
  MUST_TRUE(rc == 0, server.err_msg());
  DEFER([]() { server.Clean(); });

  // init count
  atomic<int> count{0};
  atomic<int> client_count{2};

  // init event function
  auto event_func = [&](TcpServer::Event event, TcpServer &server, Socket fd) {
    if (event == TcpServer::kEventAccept) {
      // accept
      DEBUG(fd.fd() << " accept");
    } else if (event == TcpServer::kEventRead) {
      // read
      string buf;
      auto ser_rc = fd.Read(buf, 1);
      DEBUG(fd.fd() << " read " << buf);
      // str2int
      int num = stoi(buf);
      // add count
      count += num;
      // send back
      ser_rc = fd.Write(to_string(num));
    } else if (event == TcpServer::kEventLeave) {
      // leave, break loop
      DEBUG(fd.fd() << " leave ");
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
  const int test_num = 2;
  srand(time(nullptr));
  atomic<int> sum{0};
  cpptest::WaitGroup wait_group;
  for (int i = 0; i < 2; i++) {
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
      usleep(10000);
      cli_rc = client.Close();
      EXPECT_EQ(cli_rc, 0);
      DEBUG("client exit");
    }));
  }

  // run server
  rc = server.EpollLoop();
  MUST_EQUAL(rc, 0);

  wait_group.Wait();

  // check
  DEBUG("count " << count << " sum " << sum);
  MUST_EQUAL(count, sum);
}
