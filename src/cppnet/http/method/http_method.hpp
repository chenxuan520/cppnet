#pragma once

#include <string>
namespace cppnet {

enum class HttpMethod {
  GET,
  POST,
  PUT,
  DELETE,
  HEAD,
  OPTIONS,
  PATCH,
  TRACE,
  CONNECT,
  UNKNOWN
};

class HttpMethodUtil {
public:
  /**
   * @brief: convert http method to string
   * @param method: http method
   * @return: http method string
   */
  static std::string ConvertToStr(const HttpMethod &method);
  /**
   * @brief: convert string to http method
   * @param method: http method string
   * @return: http method
   */
  static HttpMethod ConvertToMethod(const std::string &method);
};

} // namespace cppnet
