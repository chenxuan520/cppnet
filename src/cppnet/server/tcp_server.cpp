#include "tcp_server.hpp"
#include "../utils/const.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <ostream>
#include <signal.h>
#include <string.h>
#include <unistd.h>

namespace cppnet {

TcpServer::TcpServer(const std::string &ip, uint16_t port) : addr_(ip, port) {}

TcpServer::TcpServer(Address &addr) : addr_(addr) {}

TcpServer::~TcpServer() {
  Stop();
  Clean();
}

Socket TcpServer::CreateSocket() {
  Socket listenfd;
  auto rc = listenfd.Init();
  if (rc < 0) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    return kSysErr;
  }

  // set reuse addr to avoid time wait delay
  rc = listenfd.SetReuseAddr();
  if (rc < 0) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    return kSysErr;
  }

  rc = listenfd.Bind(addr_);
  if (rc < 0) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    return kSysErr;
  }

  return listenfd;
}

void TcpServer::Register(EventCallBack cb) { event_callback_ = cb; }

int TcpServer::Listen(int fd) {
  if (::listen(fd, max_connect_queue_) < 0) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    return kSysErr;
  }
  return kSuccess;
}

int TcpServer::RemoveSoc(const Socket &soc) {
  return io_multiplexing_->RemoveSoc(soc);
}

int TcpServer::AddSoc(const Socket &soc) {
  return io_multiplexing_->MonitorSoc(soc);
}

void TcpServer::HandleAccept() {
  Address addr;
  socklen_t in_len = sizeof(in_addr);

  auto new_socket = listenfd_.Accept(addr, &in_len);
  if (new_socket.status() != Socket::kInit) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    return;
  }

  // ET mode need set to non block
  auto rc = new_socket.SetNoBlock();
  if (rc < 0) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    new_socket.Close();
    return;
  }

  // add to epoll
  rc = io_multiplexing_->MonitorSoc(new_socket);
  if (rc < 0) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    new_socket.Close();
    return;
  }

  if (event_callback_) {
    event_callback_(kEventAccept, *this, new_socket);
  }
}

void TcpServer::HandleRead(int fd) {
  if (event_callback_) {
    Socket soc(fd);
    event_callback_(kEventRead, *this, soc);
  }
}

void TcpServer::HandleLeave(int fd) {
  Socket soc(fd);
  if (event_callback_) {
    event_callback_(kEventLeave, *this, soc);
  }
  RemoveSoc(soc);
  soc.Close();
}

int TcpServer::EpollLoop() {
  if (listenfd_.status() != Socket::kInit) {
    err_msg_ = "[logicerr]:epfd or listenfd not init";
    return kLogicErr;
  }

  if (io_multiplexing_ == nullptr) {
    err_msg_ = "[logicerr]:io multiplexing not init";
    return kLogicErr;
  }

  auto callback = [this](IOMultiplexingBase &, Socket fd,
                         IOMultiplexingBase::IOEvent event) {
    if (fd == listenfd_) {
      HandleAccept();
    } else if (event == IOMultiplexingBase::kIOEventLeave) {
      HandleLeave(fd.fd());
    } else if (event == IOMultiplexingBase::kIOEventRead) {
      HandleRead(fd.fd());
    }
  };

  auto rc = io_multiplexing_->Loop(callback);
  if (rc < 0) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    return kSysErr;
  }
  return kSuccess;
}

void TcpServer::Stop() { io_multiplexing_->Stop(); }

void TcpServer::Clean() {
  if (listenfd_.status() == Socket::kInit) {
    listenfd_.Close();
  }
  if (io_multiplexing_ != nullptr) {
    io_multiplexing_->Close();
  }
}

int TcpServer::Init() {
  listenfd_ = CreateSocket();
  if (listenfd_.status() != Socket::kInit) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    return kSysErr;
  }

  auto retval = listenfd_.SetNoBlock();
  if (retval < 0) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    listenfd_.Close();
    return kSysErr;
  }

  retval = listenfd_.Listen(max_connect_queue_);
  if (retval < 0) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    listenfd_.Close();
    return kSysErr;
  }

  io_multiplexing_ = IOMultiplexingFactory::CreateDefault();
  if (io_multiplexing_ == nullptr) {
    err_msg_ = "[logicerr]:io multiplexing not init";
    listenfd_.Close();
    return kLogicErr;
  }

  retval = io_multiplexing_->Init();
  if (retval < 0) {
    err_msg_ = io_multiplexing_->err_msg();
    listenfd_.Close();
    return kSysErr;
  }

  retval = io_multiplexing_->MonitorSoc(listenfd_);
  if (retval < 0) {
    err_msg_ = io_multiplexing_->err_msg();
    listenfd_.Close();
    return kSysErr;
  }
  return kSuccess;
}

} // namespace cppnet
