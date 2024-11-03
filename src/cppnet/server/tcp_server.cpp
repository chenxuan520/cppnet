#include "tcp_server.hpp"
#include "../utils/const.hpp"
#include "io_multiplexing/io_multiplexing_factory.hpp"

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
    err_msg_ = "[syserr]:" + listenfd_.err_msg();
    return kSysErr;
  }

  // set reuse addr to avoid time wait delay
  rc = listenfd.SetReuseAddr();
  if (rc < 0) {
    err_msg_ = "[syserr]:" + listenfd_.err_msg();
    return kSysErr;
  }

  // recheck ip and port addr
  std::string ip;
  uint16_t port;
  addr_.GetIPAndPort(ip, port);
  if (ip.empty() || port == 0) {
    err_msg_ = "[logicerr]:ip or port is invalid";
    return kLogicErr;
  }

  rc = listenfd.Bind(addr_);
  if (rc < 0) {
    err_msg_ = "[syserr]:" + listenfd_.err_msg();
    return kSysErr;
  }

  return listenfd;
}

void TcpServer::Register(EventCallBack cb) { event_callback_ = cb; }

int TcpServer::RemoveSoc(const Socket &soc) {
  if (io_multiplexing_ != nullptr) {
    return io_multiplexing_->RemoveSoc(soc);
  }
  return kSuccess;
}

int TcpServer::AddSoc(const Socket &soc) {
  if (soc.status() != Socket::kInit) {
    err_msg_ = "[logicerr]:soc is not init";
    return kLogicErr;
  }
  return io_multiplexing_->MonitorSoc(soc);
}

void TcpServer::HandleAccept() {
  Address addr;

  auto new_socket = listenfd_.Accept(addr);
  if (new_socket.status() != Socket::kInit) {
    err_msg_ = "[syserr]:" + new_socket.err_msg();
    event_callback_(kEventError, *this, new_socket);
    return;
  }

  switch (mode_) {

  case kMixed:
  case kIOMultiplexing: {
    auto rc = io_multiplexing_->MonitorSoc(new_socket);
    if (rc < 0) {
      err_msg_ = "[syserr]:" + io_multiplexing_->err_msg();
      event_callback_(kEventError, *this, new_socket);
      new_socket.Close();
      return;
    }
  } break;

  default:
    break;
  }

  event_callback_(kEventAccept, *this, new_socket);
}

void TcpServer::HandleRead(int fd) {
  Socket soc(fd);
  event_callback_(kEventRead, *this, soc);
}

void TcpServer::HandleLeave(int fd) {
  Socket soc(fd);
  event_callback_(kEventLeave, *this, soc);
  RemoveSoc(soc);
  soc.Close();
}

void TcpServer::HandleError(int fd) {
  Socket soc(fd);
  event_callback_(kEventError, *this, soc);
  RemoveSoc(soc);
  soc.Close();
}

int TcpServer::EventLoop() {
  if (listenfd_.status() != Socket::kInit) {
    err_msg_ = "[logicerr]:epfd or listenfd not init";
    return kLogicErr;
  }

  if (io_multiplexing_ == nullptr && mode_ != kMultiThread) {
    err_msg_ = "[logicerr]:io multiplexing not init";
    return kLogicErr;
  }

  switch (mode_) {

  case kMixed:
  case kIOMultiplexing: {
    auto callback = [this](IOMultiplexingBase &, Socket fd,
                           IOMultiplexingBase::IOEvent event) {
      if (fd == listenfd_) {
        HandleAccept();

      } else if (event == IOMultiplexingBase::kIOEventLeave) {
        HandleLeave(fd.fd());

      } else if (event == IOMultiplexingBase::kIOEventRead) {
        if (mode_ == kMixed) {
          auto task_func = std::bind(&TcpServer::HandleRead, this, fd.fd());
          thread_pool_->AddTask({task_func});
        } else {
          HandleRead(fd.fd());
        }

      } else if (event == IOMultiplexingBase::kIOEventError) {
        HandleError(fd.fd());

      } else {
        err_msg_ = "[logicerr]:unknown event";
      }
    };
    auto rc = io_multiplexing_->Loop(callback);
    if (rc < 0) {
      err_msg_ = "[syserr]:" + std::string(strerror(errno));
      return kSysErr;
    }
    if (mode_ == kMixed) {
      thread_pool_->Stop();
    }
  } break;

  case kMultiThread: {
    auto callback = [&](Socket fd) {
      event_callback_(kEventAccept, *this, fd);
      std::string tmp;
      while (fd.ReadPeek(tmp, 1) > 0) {
        event_callback_(kEventRead, *this, fd);
      }
      event_callback_(kEventLeave, *this, fd);
      return;
    };

    while (loop_flag_) {
      Address addr;
      auto accept_fd = listenfd_.Accept(addr);
      if (!loop_flag_) {
        break;
      }
      if (accept_fd.status() != Socket::kInit) {
        err_msg_ = "[syserr]:" + listenfd_.err_msg();
        event_callback_(kEventError, *this, accept_fd);
        return kSysErr;
      }

      thread_pool_->AddTask({std::bind(callback, accept_fd)});
    }
    thread_pool_->Stop();
  } break;

  default:
    err_msg_ = "[logicerr]:unknown mode " + std::to_string(mode_);
    return kLogicErr;
  }
  return kSuccess;
}

void TcpServer::Stop() {
  if (listenfd_.status() != Socket::kInit) {
    err_msg_ = "[logicerr]:epfd or listenfd not init";
    return;
  }
  loop_flag_ = false;
  switch (mode_) {
  case kMixed:
  case kIOMultiplexing: {
    io_multiplexing_->Stop();
  } break;

  default:
    break;
  }
}

void TcpServer::Clean() {
  if (listenfd_.status() == Socket::kInit) {
    listenfd_.Close();
  }
  if (io_multiplexing_ != nullptr) {
    io_multiplexing_->Close();
  }
}

int TcpServer::WakeUp() {
  if (loop_flag_) {
    err_msg_ = "[logicerr]:server is running";
    return kLogicErr;
  }

  switch (mode_) {
  case kMultiThread:
  case kMixed: {
    Socket soc;
    auto rc = soc.Init();
    if (soc.status() != Socket::kInit) {
      err_msg_ = "[syserr]:" + soc.err_msg();
      return kSysErr;
    }
    rc = soc.Connect(addr_);
    if (rc < 0) {
      err_msg_ = "[syserr]:" + soc.err_msg();
      return kSysErr;
    }
    rc = soc.Close();
    if (rc < 0) {
      err_msg_ = "[syserr]:" + soc.err_msg();
      return kSysErr;
    }
  } break;
  default:
    // io_multiplexing do not need
    break;
  }
  return kSuccess;
}

int TcpServer::Init() {
  loop_flag_ = true;
  listenfd_ = CreateSocket();
  if (listenfd_.status() != Socket::kInit) {
    return kSysErr;
  }

  auto retval = listenfd_.Listen(max_connect_queue_);
  if (retval < 0) {
    err_msg_ = "[syserr]:" + listenfd_.err_msg();
    listenfd_.Close();
    return kSysErr;
  }

  return InitMode();
}

int TcpServer::InitMode() {

  switch (mode_) {

  case kMixed: {
    thread_pool_ = std::make_shared<ThreadPool>();
    thread_pool_->Init();
  } // need init pool and io_multiplexing both
  case kIOMultiplexing: {
    io_multiplexing_ = IOMultiplexingFactory::CreateDefault();
    if (io_multiplexing_ == nullptr) {
      err_msg_ = "[logicerr]:io multiplexing not init";
      listenfd_.Close();
      return kLogicErr;
    }

    auto retval = io_multiplexing_->Init();
    if (retval < 0) {
      err_msg_ = "[syserr]:" + io_multiplexing_->err_msg();
      listenfd_.Close();
      return kSysErr;
    }

    retval = io_multiplexing_->MonitorSoc(listenfd_);
    if (retval < 0) {
      err_msg_ = "[syserr]:" + io_multiplexing_->err_msg();
      listenfd_.Close();
      return kSysErr;
    }

  } break;

  case kMultiThread: {
    thread_pool_ = std::make_shared<ThreadPool>();
    thread_pool_->Init();
  } break;

  default:
    return kSuccess;
  }

  // init default callback
  event_callback_ = [](Event, TcpServer &, Socket) {};

  return kSuccess;
}

} // namespace cppnet
