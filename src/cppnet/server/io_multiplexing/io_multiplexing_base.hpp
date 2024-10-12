#pragma once

#include "socket/socket.hpp"
#include <functional>
namespace cppnet {

/*! \enum IOMultiplexingType
 *
 * @brief: IOMultiplexing type.
 */
enum IOMultiplexingType {
  kEpoll,  // for linux
  kSelect, // for windows
  kQueue,  // for mac unix
};

class IOMultiplexingBase {
public:
  enum IOEvent {
    kIOEventRead = 1,
    kIOEventLeave = 2,
    kIOEventError = 3,
  };
  using NotifyCallBack =
      std::function<void(IOMultiplexingBase &, Socket, IOEvent)>;

public:
  /**
   * @brief: Init multiplexing.
   * @return: 0 if success, -1 if failed.
   */
  virtual int Init() = 0;
  /**
   * @brief: Monitor socket.
   * @param fd: socket to monitor.
   * @return: 0 if success, -1 if failed.
   */
  virtual int MonitorSoc(const Socket &fd) = 0;
  /**
   * @brief: Remove socket from monitor.
   * @param fd: socket to remove.
   * @return: 0 if success, -1 if failed.
   */
  virtual int RemoveSoc(const Socket &fd) = 0;
  /**
   * @brief: Event loop.
   * @param callback: callback function when event happens.will loop until stop
   * is called.
   * @return: 0 if success, -1 if failed.
   */
  virtual int Loop(NotifyCallBack callback) = 0;
  /**
   * @brief: Close multiplexing.
   */
  virtual void Close() = 0;
  /**
   * @brief: Stop loop.
   */
  virtual void Stop() { loop_flag_ = false; }

public:
  /**
   * @brief: Get error message.
   */
  inline std::string err_msg() const { return err_msg_; }
  /**
   * @brief: Set max event number.
   */
  inline void set_max_event_num(int num) { max_event_num_ = num; }
  /**
   * @brief: Set wait timeout.
   */
  inline void set_wait_timeout(int timeout) { wait_timeout_ = timeout; }

protected:
  std::string err_msg_;
  bool loop_flag_ = true;
  int max_event_num_ = 1024;
  int wait_timeout_ = -1;
};

} // namespace cppnet
