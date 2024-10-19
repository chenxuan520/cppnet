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
    std::string value = it.substr(pos + 1);
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

} // namespace cppnet
