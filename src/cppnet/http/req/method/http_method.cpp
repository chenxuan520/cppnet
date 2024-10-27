#include "http_method.hpp"

namespace cppnet {

std::string HttpMethodUtil::ConvertToStr(const HttpMethod &method) {
  switch (method) {
  case HttpMethod::GET:
    return "GET";
  case HttpMethod::POST:
    return "POST";
  case HttpMethod::PUT:
    return "PUT";
  case HttpMethod::DELETE:
    return "DELETE";
  case HttpMethod::HEAD:
    return "HEAD";
  case HttpMethod::OPTIONS:
    return "OPTIONS";
  case HttpMethod::PATCH:
    return "PATCH";
  case HttpMethod::TRACE:
    return "TRACE";
  case HttpMethod::CONNECT:
    return "CONNECT";
  default:
    return "UNKNOWN";
  }
}

HttpMethod HttpMethodUtil::ConvertToMethod(const std::string &method) {
  if (method == "GET")
    return HttpMethod::GET;
  else if (method == "POST")
    return HttpMethod::POST;
  else if (method == "PUT")
    return HttpMethod::PUT;
  else if (method == "DELETE")
    return HttpMethod::DELETE;
  else if (method == "HEAD")
    return HttpMethod::HEAD;
  else if (method == "OPTIONS")
    return HttpMethod::OPTIONS;
  else if (method == "PATCH")
    return HttpMethod::PATCH;
  else if (method == "TRACE")
    return HttpMethod::TRACE;
  else if (method == "CONNECT")
    return HttpMethod::CONNECT;
  else
    return HttpMethod::UNKNOWN;
}

} // namespace cppnet
