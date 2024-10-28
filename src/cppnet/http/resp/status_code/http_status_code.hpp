#pragma once

#include <string>
namespace cppnet {

enum class HttpStatusCode {
  OK = 200,
  CREATED = 201,
  ACCEPTED = 202,
  NO_CONTENT = 204,
  PARTIAL_CONTENT = 206,
  MOVED_PERMANENTLY = 301,
  FOUND = 302,
  BAD_REQUEST = 400,
  UNAUTHORIZED = 401,
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  METHOD_NOT_ALLOWED = 405,
  INTERNAL_SERVER_ERROR = 500,
  UNKNOWN = -1
};

class HttpStatusCodeUtil {
public:
  /**
   * @brief: convert http status code to string
   * @param code: http status code
   * @return: http status code string
   */
  static std::string ConvertToStr(const HttpStatusCode &code);
  /**
   * @brief: convert string to http status code
   * @param code: http status code string
   * @return: http status code
   */
  static HttpStatusCode ConvertToCode(const std::string &code);
  /**
   * @brief: convert int to http status code
   * @param code: http status code int
   * @return: http status code
   */
  static HttpStatusCode ConvertToCode(int code);
};

} // namespace cppnet
