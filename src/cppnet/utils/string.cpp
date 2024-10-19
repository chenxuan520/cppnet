#include "string.hpp"

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

} // namespace cppnet
