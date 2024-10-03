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
    kEventLeave = 2,
  };
  using EventCallBack = std::function<void(Event, TcpServer &, Socket)>;

  enum RC {
    kSuccess = 0,
    kSysErr = -1,
    kLogicErr = -2,
  };

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
   * @brief: Clean all resources.
   */
  void Clean();
  /**
   * @brief: Register callback function when event happens.
   */
  void Register(EventCallBack cb);
  /**
   * @brief: Epoll event loop.
   * @return: 0 if success, -1 if failed.
   */
  int EpollLoop();
  /**
   * @brief: Close file descriptor.With remove from epoll.
   */
  int RemoveSoc(const Socket &fd);
  /**
   * @brief: Add file descriptor to epoll.
   */
  int AddSoc(const Socket &fd);

public:
  inline void set_max_connect_queue(int max_connect_queue) {
    max_connect_queue_ = max_connect_queue;
  }
  inline void set_max_event_num(int max_event_num) {
    max_event_num_ = max_event_num;
  }
  inline void set_epoll_timeout(int epoll_timeout) {
    epoll_timeout_ = epoll_timeout;
  }
  inline std::string err_msg() const { return err_msg_; }

protected:
  Socket CreateEpoll();
  Socket CreateSocket();
  int Listen(int fd);
  int UpdateEpollEvents(int efd, int op, int fd, int events);
  int DeleteEpollEvents(int efd, int fd);

  void HandleAccept();
  void HandleRead(int fd);
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
  EventCallBack event_callback_{nullptr};
  // err msg
  std::string err_msg_;

private:
  // default max connect queue is 10
  int max_connect_queue_{10};
  // default max event number is 128
  int max_event_num_{128};
  // default epoll timeout is -1, epoll_wait will block until event
  int epoll_timeout_{-1};
};

} // namespace cppnet
