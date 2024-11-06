#pragma once
#include <string>
namespace cppnet {

class Host {
public:
  /**
   * @brief: get local ip
   * @return: local ip
   */
  static std::string GetLocalIP();
  /**
   * @brief: get local name
   * @return: local name
   */
  static std::string GetLocalName();
};
} // namespace cppnet
