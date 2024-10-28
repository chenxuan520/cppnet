#include "http_status_code.hpp"

namespace cppnet {

std::string HttpStatusCodeUtil::ConvertToStr(const HttpStatusCode &code) {
  switch (code) {
  case HttpStatusCode::OK:
    return "OK";
  case HttpStatusCode::CREATED:
    return "Created";
  case HttpStatusCode::ACCEPTED:
    return "Accepted";
  case HttpStatusCode::NO_CONTENT:
    return "No Content";
  case HttpStatusCode::PARTIAL_CONTENT:
    return "Partial Content";
  case HttpStatusCode::BAD_REQUEST:
    return "Bad Request";
  case HttpStatusCode::UNAUTHORIZED:
    return "Unauthorized";
  case HttpStatusCode::FORBIDDEN:
    return "Forbidden";
  case HttpStatusCode::NOT_FOUND:
    return "Not Found";
  case HttpStatusCode::METHOD_NOT_ALLOWED:
    return "Method Not Allowed";
  case HttpStatusCode::INTERNAL_SERVER_ERROR:
    return "Internal Server Error";
  case HttpStatusCode::MOVED_PERMANENTLY:
    return "Moved Permanently";
  case HttpStatusCode::FOUND:
    return "Found";
  default:
    return "Unknown";
  }
}

HttpStatusCode HttpStatusCodeUtil::ConvertToCode(const std::string &code) {
  if (code == "OK") {
    return HttpStatusCode::OK;
  } else if (code == "Created") {
    return HttpStatusCode::CREATED;
  } else if (code == "Accepted") {
    return HttpStatusCode::ACCEPTED;
  } else if (code == "No Content") {
    return HttpStatusCode::NO_CONTENT;
  } else if (code == "Partial Content") {
    return HttpStatusCode::PARTIAL_CONTENT;
  } else if (code == "Bad Request") {
    return HttpStatusCode::BAD_REQUEST;
  } else if (code == "Unauthorized") {
    return HttpStatusCode::UNAUTHORIZED;
  } else if (code == "Forbidden") {
    return HttpStatusCode::FORBIDDEN;
  } else if (code == "Not Found") {
    return HttpStatusCode::NOT_FOUND;
  } else if (code == "Method Not Allowed") {
    return HttpStatusCode::METHOD_NOT_ALLOWED;
  } else if (code == "Internal Server Error") {
    return HttpStatusCode::INTERNAL_SERVER_ERROR;
  } else if (code == "Moved Permanently") {
    return HttpStatusCode::MOVED_PERMANENTLY;
  } else if (code == "Found") {
    return HttpStatusCode::FOUND;
  } else {
    return HttpStatusCode::UNKNOWN;
  }
}

HttpStatusCode HttpStatusCodeUtil::ConvertToCode(int code) {
  switch (code) {
  case 200:
    return HttpStatusCode::OK;
  case 201:
    return HttpStatusCode::CREATED;
  case 202:
    return HttpStatusCode::ACCEPTED;
  case 204:
    return HttpStatusCode::NO_CONTENT;
  case 206:
    return HttpStatusCode::PARTIAL_CONTENT;
  case 400:
    return HttpStatusCode::BAD_REQUEST;
  case 401:
    return HttpStatusCode::UNAUTHORIZED;
  case 403:
    return HttpStatusCode::FORBIDDEN;
  case 404:
    return HttpStatusCode::NOT_FOUND;
  case 405:
    return HttpStatusCode::METHOD_NOT_ALLOWED;
  case 500:
    return HttpStatusCode::INTERNAL_SERVER_ERROR;
  case 301:
    return HttpStatusCode::MOVED_PERMANENTLY;
  case 302:
    return HttpStatusCode::FOUND;
  default:
    return HttpStatusCode::UNKNOWN;
  }
}

} // namespace cppnet
