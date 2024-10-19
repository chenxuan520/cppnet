#pragma once

#include <string>
#include <unordered_map>

namespace cppnet {

class HttpHeader {
public:
  HttpHeader() = default;

  /**
   * @brief: parse header
   * @param origin_header: origin header
   * @return: 0: success, -1: invalid header
   */
  int Parse(const std::string &origin_header);

  /**
   * @brief: add header
   * @param key: header key
   * @param value: header value
   */
  void Add(const std::string &key, const std::string &value);

  /**
   * @brief: get header
   * @param key: header key
   * @return: header value
   */
  std::string Get(const std::string &key) const;

  /**
   * @brief: convert to string
   * @return: header string
   */
  std::string ToString() const;

  /**
   * @brief: get header
   * @param key: header key
   * @return: header value
   */
  std::string &operator[](const std::string &key);

  /**
   * @brief: get error message
   */
  inline std::string err_msg() { return err_msg_; }

private:
  std::unordered_map<std::string, std::string> headers_;

  std::string err_msg_;
};

} // namespace cppnet
