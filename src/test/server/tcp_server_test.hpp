#include "server/tcp_server.hpp"
#include "test.h"

using namespace cppnet;
using namespace std;

class TestServer : public cpptest::Test {
public:
  void SetUp() override {
    auto rc = server_.Init();
    MUST_EQUAL(rc, 0);
  }

public:
  Address addr_{"127.0.0.1", 8080};
  TcpServer server_{addr_};
};

TEST_F(TestServer, TcpServer) {
  // init msg
  string msg = "hello world";

  // init client
  Socket client;
  auto rc = client.Init();
  MUST_EQUAL(rc, 0);
  DEBUG("client fd: " << client.fd());

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
  server_.Register(event_func);

  // thread to run client
  GO_JOIN([&]() {
    // connect
    auto cli_rc = client.Connect(addr_);
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
  server_.EpollLoop();
}
