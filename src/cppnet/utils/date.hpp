#pragma once

#include <string>

namespace cppnet {
class Date {
public:
  /*
   * @brief Get the current date and time in the specified format
   * @param format The format of the date and time
   * @return The current date and time in the specified format
   * @note The default format is "%Y/%m/%d %H:%M:%S"
   */
  static std::string GetNow(std::string format = "%Y/%m/%d %H:%M:%S");
};
} // namespace cppnet
