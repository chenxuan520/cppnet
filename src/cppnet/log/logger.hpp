#pragma once

#include <iostream>
#include <string>
namespace cppnet {

// TODO: add color //
class Logger {
public:
  Logger() = default;
  virtual ~Logger() = default;
  virtual void Debug(const std::string &msg) {
    std::cout << "[DEBUG]:" << msg << std::endl;
  }
  virtual void Info(const std::string &msg) {
    std::cout << "[INFO]:" << msg << std::endl;
  }
  virtual void Warn(const std::string &msg) {
    std::cout << "[WARN]:" << msg << std::endl;
  }
  virtual void Error(const std::string &msg) {
    std::cerr << "[ERROR]:" << msg << std::endl;
  }
};

} // namespace cppnet
