#pragma once

#include <string>

namespace cppnet {

class File {
public:
  /**
   * @brief: read file
   * @param path: file path
   * @param data: file data
   * @return: 0 success, -1 fail
   */
  static int Read(const std::string &path, std::string &data);
  /**
   * @brief: write file
   * @param path: file path
   * @param data: file data
   * @return: 0 success, -1 fail
   */
  static int Write(const std::string &path, const std::string &data);
  /**
   * @brief: check file exist
   * @param path: file path
   * @return: true exist, false not exist,
   * @note: dir return false
   */
  static bool Exist(const std::string &path);
  /**
   * @brief: get file suffix, not include .
   * @param path: file path
   * @return: file suffix
   */
  static std::string Suffix(const std::string &path);
};

} // namespace cppnet
