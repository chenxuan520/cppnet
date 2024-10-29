#pragma once

#include <string>
namespace cppnet {

class Logger {
public:
  Logger() = default;
  virtual void Debug(const std::string &msg) {}
  virtual void Info(const std::string &msg) {}
  virtual void Warn(const std::string &msg) {}
  virtual void Error(const std::string &msg) {}
};

} // namespace cppnet
