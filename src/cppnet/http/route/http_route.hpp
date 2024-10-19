#pragma once

#include <string>
#include <unordered_map>

namespace cppnet {

class HttpRoute {
public:
  HttpRoute() = default;

  /**
   * @brief: parse path
   * @param origin_path: origin path
   */
  int Parse(const std::string &origin_path);

  /**
   * @brief: add param
   * @param key: param key
   * @param value: param value
   */
  void AddParam(const std::string &key, const std::string &value);

  /**
   * @brief: get param
   * @param key: param key
   * @return: param value
   */
  std::string GetParam(const std::string &key) const;

  /**
   * @brief: set path
   * @param path: path
   */
  inline void SetPath(const std::string &path) { path_ = path; }

  /**
   * @brief: get path
   * @return: path
   */
  inline std::string GetPath() const { return path_; }

  /**
   * @brief: convert to string
   * @return: route string
   */
  std::string ToString() const;

  /**
   * @brief: get param
   */
  inline std::string &operator[](const std::string &key) {
    return params_[key];
  }

  /**
   * @brief: get error message
   */
  inline std::string err_msg() { return err_msg_; }

private:
  std::string path_;
  std::string err_msg_;
  std::unordered_map<std::string, std::string> params_;
};

} // namespace cppnet
