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

void HttpHeader::Clear() { headers_.clear(); }

HttpHeader::ContentType
HttpHeader::ConvertToContentType(const std::string &content_type) {
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
  else if (content_type == "image/png")
    return HttpHeader::ContentType::kImagePng;
  else if (content_type == "image/jpeg")
    return HttpHeader::ContentType::kImageJpeg;
  else if (content_type == "image/gif")
    return HttpHeader::ContentType::kImageGif;
  else if (content_type == "image/webp")
    return HttpHeader::ContentType::kImageWebp;
  else
    return HttpHeader::ContentType::kUnknown;
}

HttpHeader::ContentType
HttpHeader::ConvertFileType(const std::string &file_type) {
  if (file_type == "html" || file_type == "htm")
    return HttpHeader::ContentType::kTextHtml;
  else if (file_type == "txt")
    return HttpHeader::ContentType::kTextPlain;
  else if (file_type == "css" || file_type == "less")
    return HttpHeader::ContentType::kTextCss;
  else if (file_type == "js" || file_type == "mjs")
    return HttpHeader::ContentType::kTextJavascript;
  else if (file_type == "json")
    return HttpHeader::ContentType::kApplicationJson;
  else if (file_type == "png")
    return HttpHeader::ContentType::kImagePng;
  else if (file_type == "jpg" || file_type == "jpeg")
    return HttpHeader::ContentType::kImageJpeg;
  else if (file_type == "gif")
    return HttpHeader::ContentType::kImageGif;
  else if (file_type == "webp")
    return HttpHeader::ContentType::kImageWebp;
  else
    return HttpHeader::ContentType::kApplicationOctetStream;
}

std::string
HttpHeader::ConvertToStr(const HttpHeader::ContentType &content_type) {
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
  case HttpHeader::ContentType::kImagePng:
    return "image/png";
  case HttpHeader::ContentType::kImageJpeg:
    return "image/jpeg";
  case HttpHeader::ContentType::kImageGif:
    return "image/gif";
  case HttpHeader::ContentType::kImageWebp:
    return "image/webp";
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

void HttpHeader::SetContentType(const std::string &content_type) {
  headers_["Content-Type"] = content_type;
}

void HttpHeader::SetHost(const std::string &host) { headers_["Host"] = host; }

void HttpHeader::SetLongConnection(bool is_long_connect) {
  if (is_long_connect) {
    headers_["Connection"] = "keep-alive";
  } else {
    headers_["Connection"] = "close";
  }
}

void HttpHeader::SetCookie(const Cookie &cookie) {
  std::string cookie_str = cookie.key + "=" + cookie.value;
  if (!cookie.expires.empty()) {
    cookie_str += "; expires=" + cookie.expires;
  }
  if (!cookie.path.empty()) {
    cookie_str += "; path=" + cookie.path;
  }
  if (!cookie.domain.empty()) {
    cookie_str += "; domain=" + cookie.domain;
  }
  if (cookie.secure) {
    cookie_str += "; secure";
  }
  if (cookie.http_only) {
    cookie_str += "; HttpOnly";
  }
  if (headers_.find("Set-Cookie") == headers_.end()) {
    headers_["Set-Cookie"] = cookie_str;
  } else {
    headers_["Set-Cookie"] += "; " + cookie_str;
  }
}

std::string HttpHeader::GetCookieVal(const std::string &key) const {
  auto it = headers_.find("Cookie");
  if (it == headers_.end()) {
    return "";
  }
  std::vector<std::string> cookies;
  StringUtil::Split(it->second, ";", cookies);
  for (auto &cookie : cookies) {
    size_t pos = cookie.find("=");
    if (pos == std::string::npos) {
      continue;
    }
    auto space_offset = cookie.find_first_not_of(" ");
    std::string cookie_key = cookie.substr(space_offset, pos - space_offset);
    std::string cookie_val = cookie.substr(pos + 1);
    if (cookie_key == key) {
      return cookie_val;
    }
  }
  return "";
}

} // namespace cppnet
