#pragma once

#include "logger.hpp"
#include <memory>
#include <string>
#include <vector>
namespace cppnet {

class MultiLogger : public Logger {
public:
  MultiLogger() = default;
  void Init(std::initializer_list<std::shared_ptr<Logger>> loggers);
  virtual void Debug(const std::string &msg) override;
  virtual void Info(const std::string &msg) override;
  virtual void Warn(const std::string &msg) override;
  virtual void Error(const std::string &msg) override;
  virtual void Fatal(const std::string &msg) override;

private:
  std::vector<std::shared_ptr<Logger>> loggers_;
};

} // namespace cppnet
