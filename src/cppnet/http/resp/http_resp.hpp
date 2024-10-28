#pragma once

#include "../header/http_header.hpp"
#include "../version/http_version.hpp"
#include "status_code/http_status_code.hpp"
#include <string>

namespace cppnet {

class HttpResp {
public:
  HttpResp() = default;

public:
  /**
   * @brief: parse http response
   * @param origin_resp: origin http response
   */
  int Parse(const std::string &origin_resp);
  /**
   * @brief: set http status code
   */
  HttpVersion &version() { return version_; }
  /**
   * @brief: set http header
   */
  HttpHeader &header() { return header_; }
  /**
   * @brief: set http body
   */
  std::string &body() { return body_; }
  /**
   * @brief: set status code
   */
  HttpStatusCode &status_code() { return status_code_; }
  /**
   * @brief: convert http response to string
   */
  int Build(std::string &resp_str);
  /**
   * @brief: clear http response
   */
  void Clear();

public:
  /**
   * @brief: set http status code
   */
  void NotFound();
  void Redirect(const std::string &location, bool forever = false);
  void SuccessWithBinary(const std::string &body);
  void SuccessWithJson(const std::string &body);
  /**
   * @brief: set resp file type
   */
  void Json(HttpStatusCode status_code, const std::string &body);
  void Text(HttpStatusCode status_code, const std::string &body);

public:
  std::string err_msg() { return err_msg_; }

private:
  HttpStatusCode status_code_ = HttpStatusCode::UNKNOWN;
  HttpHeader header_;
  HttpVersion version_;
  std::string body_;
  std::string err_msg_;
};

} // namespace cppnet
