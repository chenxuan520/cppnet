#pragma once
#include "io_multiplexing_base.hpp"

namespace cppnet {

class KQueue : public IOMultiplexingBase {
public:
  KQueue();
  virtual ~KQueue();
  int Init() override;
  int MonitorSoc(const Socket &fd) override;
  int RemoveSoc(const Socket &fd) override;
  int Loop(NotifyCallBack callback) override;
  void Close() override;

private:
  Socket kq_fd_;

};

} // namespace cppnet

