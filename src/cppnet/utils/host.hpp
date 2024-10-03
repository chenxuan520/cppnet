#pragma once
#include <string>
namespace cppnet {

class Host {
public:
  static std::string GetLocalIP();
  static std::string GetLocalName();
};
} // namespace cppnet
