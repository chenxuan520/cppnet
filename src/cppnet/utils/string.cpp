#include "string.hpp"
#include <cuchar>
#include <uchar.h>

namespace cppnet {

void StringUtil::Split(const std::string &origin_str,
                       const std::string &delimiter,
                       std::vector<std::string> &arr, bool keep_delimiter) {
  size_t pos = 0;
  size_t found;
  while ((found = origin_str.find(delimiter, pos)) != std::string::npos) {
    if (keep_delimiter)
      arr.push_back(origin_str.substr(pos, found - pos + delimiter.length()));
    else
      arr.push_back(origin_str.substr(pos, found - pos));
    pos = found + delimiter.length();
  }
  arr.push_back(origin_str.substr(pos));
}

std::string StringUtil::UrlDecode(const std::string &src) {
  std::string strTemp = "";
  size_t length = src.length();
  auto FromHex = [](unsigned char ch) -> unsigned char {
    if (ch >= 'A' && ch <= 'Z')
      ch = ch - 'A' + 10;
    else if (ch >= 'a' && ch <= 'z')
      ch = ch - 'a' + 10;
    else if (ch >= '0' && ch <= '9')
      ch = ch - '0';
    else
      return 0;
    return static_cast<unsigned char>(ch);
  };
  for (size_t i = 0; i < length; i++) {
    if (src[i] == '+')
      strTemp += ' ';
    else if (src[i] == '%') {
      if (i + 2 > length) {
        return "";
      }
      unsigned char high = FromHex((unsigned char)src[++i]);
      unsigned char low = FromHex((unsigned char)src[++i]);
      strTemp += high * 16 + low;
    } else
      strTemp += src[i];
  }
  return strTemp;
}

std::string StringUtil::UrlEncode(const std::string &src) {
  std::string strTemp = "";
  size_t length = src.length();
  auto ToHex = [](unsigned char ch) -> char {
    if (ch < 10)
      return ch + '0';
    else
      return ch - 10 + 'A';
  };
  for (size_t i = 0; i < length; i++) {
    if (isalnum((unsigned char)src[i]) || (src[i] == '-') || (src[i] == '_') ||
        (src[i] == '.') || (src[i] == '~') || (src[i] == '/') ||
        (src[i] == '=') || (src[i] == '?') || (src[i] == '&'))
      strTemp += src[i];
    else if (src[i] == ' ')
      strTemp += "+";
    else {
      strTemp += '%';
      strTemp += ToHex((unsigned char)src[i] >> 4);
      strTemp += ToHex((unsigned char)src[i] % 16);
    }
  }
  return strTemp;
}

} // namespace cppnet
