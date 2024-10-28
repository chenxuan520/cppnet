#include "http_status_code.hpp"

namespace cppnet {

std::string HttpStatusCodeUtil::ConvertToStr(const HttpStatusCode &code) {
  switch (code) {
  case HttpStatusCode::OK:
    return "OK";
  case HttpStatusCode::CREATED:
    return "CREATED";
  case HttpStatusCode::ACCEPTED:
    return "ACCEPTED";
  case HttpStatusCode::NO_CONTENT:
    return "NO_CONTENT";
  case HttpStatusCode::PARTIAL_CONTENT:
    return "PARTIAL_CONTENT";
  case HttpStatusCode::BAD_REQUEST:
    return "BAD_REQUEST";
  case HttpStatusCode::UNAUTHORIZED:
    return "UNAUTHORIZED";
  case HttpStatusCode::FORBIDDEN:
    return "FORBIDDEN";
  case HttpStatusCode::NOT_FOUND:
    return "NOT_FOUND";
  case HttpStatusCode::METHOD_NOT_ALLOWED:
    return "METHOD_NOT_ALLOWED";
  case HttpStatusCode::INTERNAL_SERVER_ERROR:
    return "INTERNAL_SERVER_ERROR";
  case HttpStatusCode::MOVED_PERMANENTLY:
    return "MOVED_PERMANENTLY";
  case HttpStatusCode::FOUND:
    return "FOUND";
  default:
    return "UNKNOWN";
  }
}

HttpStatusCode HttpStatusCodeUtil::ConvertToCode(const std::string &code) {
  if (code == "OK")
    return HttpStatusCode::OK;
  else if (code == "CREATED")
    return HttpStatusCode::CREATED;
  else if (code == "ACCEPTED")
    return HttpStatusCode::ACCEPTED;
  else if (code == "NO_CONTENT")
    return HttpStatusCode::NO_CONTENT;
  else if (code == "PARTIAL_CONTENT")
    return HttpStatusCode::PARTIAL_CONTENT;
  else if (code == "BAD_REQUEST")
    return HttpStatusCode::BAD_REQUEST;
  else if (code == "UNAUTHORIZED")
    return HttpStatusCode::UNAUTHORIZED;
  else if (code == "FORBIDDEN")
    return HttpStatusCode::FORBIDDEN;
  else if (code == "NOT_FOUND")
    return HttpStatusCode::NOT_FOUND;
  else if (code == "METHOD_NOT_ALLOWED")
    return HttpStatusCode::METHOD_NOT_ALLOWED;
  else if (code == "INTERNAL_SERVER_ERROR")
    return HttpStatusCode::INTERNAL_SERVER_ERROR;
  else if (code == "MOVED_PERMANENTLY")
    return HttpStatusCode::MOVED_PERMANENTLY;
  else if (code == "FOUND")
    return HttpStatusCode::FOUND;
  else
    return HttpStatusCode::UNKNOWN;
}

} // namespace cppnet
