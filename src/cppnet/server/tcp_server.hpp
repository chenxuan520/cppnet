#pragma once

#include "../socket/socket.hpp"
#include "../utils/threadpoll.hpp"
#include "io_multiplexing/io_multiplexing_base.hpp"
#include <functional>
#include <memory>
#include <string>
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
    kEventError = 3,
  };
  using EventCallBack = std::function<void(Event, TcpServer &, Socket)>;

  /**
   * @brief: IOMultiplexing mode.
   */
  enum Mode {
    kIOMultiplexing,
    kMultiThread,
    kMixed,
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
   * @brief: Event loop.
   * @return: 0 if success, -1 if failed.
   */
  int EventLoop();
  /**
   * @brief: Close file descriptor.With remove from epoll.
   */
  int RemoveSoc(const Socket &fd);
  /**
   * @brief: Add file descriptor to epoll.
   */
  int AddSoc(const Socket &fd);
  /**
   * @brief: Wake up server,use after stop optional to quit
   */
  int WakeUp();

public:
  inline void set_mode(Mode mode) { mode_ = mode; }
  inline void set_max_connect_queue(int max_connect_queue) {
    max_connect_queue_ = max_connect_queue;
  }
  inline std::string err_msg() const { return err_msg_; }
  inline std::shared_ptr<IOMultiplexingBase> io_multiplexing() {
    return io_multiplexing_;
  }
  inline std::shared_ptr<ThreadPool<Socket>> thread_pool() {
    return thread_pool_;
  }

protected:
  Socket CreateSocket();
  int InitMode();
  int Listen(int fd);

  void HandleAccept();
  void HandleRead(int fd);
  void HandleLeave(int fd);
  void HandleError(int fd);

private:
  // server address
  Address addr_;
  // server file descriptor
  Socket listenfd_{-1};
  // epoll loop flag, if false then exit
  bool loop_flag_{true};
  // event callback function
  EventCallBack event_callback_{nullptr};
  // err msg
  std::string err_msg_;
  // server mode
  Mode mode_{kIOMultiplexing};
  // io multiplexing
  std::shared_ptr<IOMultiplexingBase> io_multiplexing_{nullptr};
  // threadpool
  std::shared_ptr<ThreadPool<Socket>> thread_pool_{nullptr};

private:
  // default max connect queue is 10
  int max_connect_queue_{10};
};

} // namespace cppnet
