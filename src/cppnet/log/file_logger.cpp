#include "file_logger.hpp"
#include "../utils/const.hpp"
#include "../utils/file.hpp"

namespace cppnet {

int FileLogger::Init(const std::string &file_path) {
  file_path_ = file_path;
  // is exist file_path_, check write permission
  // if not exist, create file
  auto rc = File::Create(file_path_);
  if (rc != kSuccess) {
    return rc;
  }
  rc = File::CanWrite(file_path_);
  if (!rc) {
    return rc;
  }
  return kSuccess;
}

} // namespace cppnet
