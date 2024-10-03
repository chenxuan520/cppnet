#pragma once

#include "../socket/socket.hpp"
#include <errno.h>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
namespace cppnet {

class TcpServer {
public:
  /**
   * @brief: Event enum.
   */
  enum Event {
    kEventAccept = 0,
    kEventRead = 1,
    kEventWrite = 2,
    kEventLeave = 3,
  };
  using EventCallBack = std::function<void(Event, TcpServer &, Socket)>;

public:
  TcpServer() = default;
  TcpServer(const std::string &ip, uint16_t port);
  TcpServer(Address &addr);
  virtual ~TcpServer();
  /**
   * @brief: Init tcp server. Create epfd, bind serverfd, set serverfd non block
   * and register serverfd to epoll.
   */
  int Init();
  /**
   * @brief: Closes tcp server. Close epfd and serverfd.
   */
  void Stop();
  /**
   * @brief: Register callback function when event happens.
   */
  void Register(EventCallBack cb);
  /**
   * @brief: Epoll event loop.
   */
  void EpollLoop();

protected:
  Socket CreateEpoll();
  Socket CreateSocket();
  int Listen(int fd);
  int UpdateEpollEvents(int efd, int op, int fd, int events);
  int DeleteEpollEvents(int efd, int fd);
  int CloseFd(int fd);

  void HandleAccept();
  void HandleRead(int fd);
  void HandleWrite(int fd);
  void HandleLeave(int fd);

private:
  // server address
  Address addr_;
  // epoll file descriptor
  Socket epfd_{-1};
  // server file descriptor
  Socket listenfd_{-1};
  // epoll loop flag, if false then exit
  bool loop_flag_{true};
  // event callback function
  EventCallBack event_callback_;

private:
  int max_connect_queue_{10};
  int max_event_num_{10};
  int epoll_timeout_{-1};
};

} // namespace cppnet
