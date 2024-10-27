#include "http_header.hpp"
#include "../../utils/const.hpp"
#include "../../utils/string.hpp"
#include <vector>

namespace cppnet {

int HttpHeader::Parse(const std::string &origin_header) {
  if (origin_header.empty()) {
    err_msg_ = "header is empty";
    return kLogicErr;
  }
  // split with \r\n
  std::vector<std::string> single_header;
  const std::string delimiter = "\r\n";
  StringUtil::Split(origin_header, delimiter, single_header);
  // parse header
  for (auto &it : single_header) {
    size_t pos = it.find(":");
    if (pos == std::string::npos) {
      err_msg_ = "header format error";
      return kLogicErr;
    }
    std::string key = it.substr(0, pos);
    std::string value =
        it.substr(pos + 1 + it.substr(pos + 1).find_first_not_of(" "));
    Add(key, value);
  }
  return kSuccess;
}

void HttpHeader::Add(const std::string &key, const std::string &value) {
  headers_[key] = value;
}

std::string HttpHeader::Get(const std::string &key) const {
  auto it = headers_.find(key);
  if (it != headers_.end())
    return it->second;
  else
    return "";
}

std::string HttpHeader::ToString() const {
  std::string ret;
  for (auto &it : headers_) {
    ret += it.first + ": " + it.second + "\r\n";
  }
  ret += "\r\n";
  return ret;
}

std::string &HttpHeader::operator[](const std::string &key) {
  return headers_[key];
}

HttpHeader::ContentType
HttpHeader::ConvertToContentType(const std::string &content_type) const {
  if (content_type == "text/html")
    return HttpHeader::ContentType::kTextHtml;
  else if (content_type == "text/plain")
    return HttpHeader::ContentType::kTextPlain;
  else if (content_type == "text/css")
    return HttpHeader::ContentType::kTextCss;
  else if (content_type == "text/javascript")
    return HttpHeader::ContentType::kTextJavascript;
  else if (content_type == "application/octet-stream")
    return HttpHeader::ContentType::kApplicationOctetStream;
  else if (content_type == "application/json")
    return HttpHeader::ContentType::kApplicationJson;
  else
    return HttpHeader::ContentType::kUnknown;
}

std::string
HttpHeader::ConvertToStr(const HttpHeader::ContentType &content_type) const {
  switch (content_type) {
  case HttpHeader::ContentType::kTextPlain:
    return "text/plain";
  case HttpHeader::ContentType::kTextHtml:
    return "text/html";
  case HttpHeader::ContentType::kTextCss:
    return "text/css";
  case HttpHeader::ContentType::kTextJavascript:
    return "text/javascript";
  case HttpHeader::ContentType::kApplicationJson:
    return "application/json";
  case HttpHeader::ContentType::kApplicationOctetStream:
    return "application/octet-stream";
  default:
    return "unknown";
  }
}

HttpHeader::ContentType HttpHeader::GetContentType() const {
  auto it = headers_.find("Content-Type");
  if (it == headers_.end()) {
    return HttpHeader::ContentType::kUnknown;
  } else {
    return ConvertToContentType(it->second);
  }
}

int HttpHeader::GetContentLength() const {
  auto it = headers_.find("Content-Length");
  if (it == headers_.end()) {
    return -1;
  } else {
    try {
      return std::stoi(it->second);
    } catch (...) {
      // fail to convert
      return -1;
    }
  }
}

std::string HttpHeader::GetHost() const {
  auto it = headers_.find("Host");
  if (it == headers_.end()) {
    return "";
  } else {
    return it->second;
  }
}

void HttpHeader::SetContentLength(int content_length) {
  headers_["Content-Length"] = std::to_string(content_length);
}

void HttpHeader::SetContentType(HttpHeader::ContentType content_type) {
  headers_["Content-Type"] = ConvertToStr(content_type);
}

void HttpHeader::SetHost(const std::string &host) { headers_["Host"] = host; }

} // namespace cppnet
