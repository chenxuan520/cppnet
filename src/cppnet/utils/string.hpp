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

private:
};

} // namespace cppnet
