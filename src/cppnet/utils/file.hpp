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
  /**
   * @brief: create file
   * @param path: file path
   * @return: 0 success, -1 fail
   * @note: auto create dir if not exist
   */
  static int Create(const std::string &path);
  /**
   * @brief: remove file
   * @param path: file path
   * @return: 0 success, -1 fail
   */
  static int Remove(const std::string &path);
  /**
   * @brief: append file
   * @param path: file path
   * @param data: file data
   * @return: 0 success, -1 fail
   */
  static int Append(const std::string &path, const std::string &data);
  /**
   * @brief: check file write permission
   * @param path: file path
   * @return: true can write, false can not write
   */
  static bool CanWrite(const std::string &path);
  /**
   * @brief: get file line count
   * @param path: file path
   * @return: line count
   * @note: file not exist return 0
   */
  static int LineCount(const std::string &path);
  /**
   * @brief: judge path is dir
   * @param path: file path
   * @return: true is dir, false not dir
   */
  static bool IsDir(const std::string &path);
  /**
   * @brief: judge path is file
   * @param path: file path
   * @return: true is file, false not file
   */
  static bool IsFile(const std::string &path);
};

} // namespace cppnet
