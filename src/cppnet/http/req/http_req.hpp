#pragma once
#include "../header/http_header.hpp"
#include "../version/http_version.hpp"
#include "method/http_method.hpp"
#include "route/http_route.hpp"

namespace cppnet {

class HttpReq {
public:
  HttpReq() = default;
  /**
   * @brief: parse http request
   * @param origin_req: origin http request
   * @return: 0: success, -1: invalid http request
   */
  int Parse(const std::string &origin_req);

  /**
   * @brief: build http request
   * @param req_str: http request string
   * @return: 0: success, -1: invalid http request
   */
  int Build(std::string &req_str);
  /**
   * @brief: clear http request
   */
  void Clear();

public:
  using KVMappings = std::unordered_map<std::string, std::string>;
  // build common http request
  void GET(const std::string &url, const KVMappings &params = KVMappings(),
           const KVMappings &extra_header = KVMappings());

  void POST(const std::string &url, const std::string &body,
            const KVMappings &extra_header = KVMappings());

  void POST(const std::string &url, const KVMappings &params,
            const KVMappings &extra_header = KVMappings());

public:
  HttpHeader &header() { return header_; }
  HttpRoute &route() { return route_; }
  HttpMethod &method() { return method_; }
  HttpVersion &version() { return version_; }
  std::string &body() { return body_; }
  std::string err_msg() { return err_msg_; }

private:
  HttpMethod method_ = HttpMethod::UNKNOWN;
  HttpVersion version_ = HttpVersion::HTTP_1_1;
  HttpHeader header_;
  HttpRoute route_;
  std::string body_;
  std::string err_msg_;
};

} // namespace cppnet
