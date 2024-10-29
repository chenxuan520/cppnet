#pragma once

#include "logger.hpp"
#include <iostream>
#include <string>
namespace cppnet {

class StdLogger : public Logger {
public:
  StdLogger() = default;
  virtual void Debug(const std::string &msg) override {
    std::cout << "[DEBUG]:" << msg << std::endl;
  }
  virtual void Info(const std::string &msg) override {
    std::cout << "[INFO]:" << msg << std::endl;
  }
  virtual void Warn(const std::string &msg) override {
    std::cout << "[WARN]:" << msg << std::endl;
  }
  virtual void Error(const std::string &msg) override {
    std::cerr << "[ERROR]:" << msg << std::endl;
  }
};

} // namespace cppnet
