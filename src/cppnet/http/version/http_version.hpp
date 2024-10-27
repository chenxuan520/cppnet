#pragma once

#include <string>
namespace cppnet {

enum class HttpVersion { HTTP_1_0, HTTP_1_1, HTTP_2_0, UNKNOWN };

class HttpVersionUtil {
public:
  /**
   * @brief: convert http version to string
   * @param version: http version
   * @return: http version string
   */
  static std::string ConvertToStr(const HttpVersion &version);
  /**
   * @brief: convert string to http version
   * @param version: http version string
   * @return: http version
   */
  static HttpVersion ConvertToVersion(const std::string &version);
};

} // namespace cppnet
