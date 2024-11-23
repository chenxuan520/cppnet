#pragma once

#include <string>
namespace cppnet {

// The version of cppnet, use in library
// if use source code, it will be unknown and the date will be 0000-00-00
class Version {
public:
  /*
   * @brief Get the version of cppnet
   * @return The version of cppnet
   */
  static std::string GetStr();
  /*
   * @brief Get the version release date of cppnet
   * @return The date of cppnet, format: "yyyy-mm-dd"
   * @note The date is the last time the version was updated
   */
  static std::string GetDate();
};

} // namespace cppnet
