#pragma once

#include "../header/http_header.hpp"
#include "../status_code/http_status_code.hpp"
#include "../version/http_version.hpp"
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
   * @brief: convert http response to string
   */
  int Build(std::string &resp);

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
