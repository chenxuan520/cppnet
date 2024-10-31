#pragma once

#include <cstring>
#include <string>
#include <vector>

namespace cppnet {

class StringUtil {
public:
  /**
   * @brief: split string
   * @param origin_str: origin string
   * @param delimiter: delimiter
   * @param arr: split result
   * @param keep_delimiter: if keep delimiter
   */
  static void Split(const std::string &origin_str, const std::string &delimiter,
                    std::vector<std::string> &arr, bool keep_delimiter = false);

  /**
   * @brief: url deccode
   * @param url: url
   * @return: decode  str
   */
  static std::string UrlDecode(const std::string &url);

  /**
   * @brief: url encode
   * @param url: utf8 string
   * @return: encode code str
   */
  static std::string UrlEncode(const std::string &url);

private:
};

} // namespace cppnet
