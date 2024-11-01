#include "file.hpp"
#include "utils/const.hpp"

#include <filesystem>
#include <fstream>
#include <unistd.h>

namespace cppnet {

int File::Read(const std::string &path, std::string &data) {
  std::ifstream ifs(path, std::ios::in | std::ios::binary);
  if (!ifs.is_open()) {
    return kLogicErr;
  }

  data.assign((std::istreambuf_iterator<char>(ifs)),
              (std::istreambuf_iterator<char>()));
  ifs.close();
  return kSuccess;
}

int File::Write(const std::string &path, const std::string &data) {
  std::ofstream ofs(path, std::ios::out | std::ios::binary);
  if (!ofs.is_open()) {
    return kLogicErr;
  }
  ofs << data;
  ofs.close();
  return kSuccess;
}

bool File::Exist(const std::string &path) {
  return std::filesystem::exists(std::filesystem::path(path)) &&
         std::filesystem::is_regular_file(std::filesystem::path(path));
}

std::string File::Suffix(const std::string &path) {
  auto last_slash = path.find_last_of("/");
  if (last_slash == std::string::npos) {
    last_slash = 0;
  }
  auto tmp = path.substr(last_slash);
  auto pos = tmp.find_last_of(".");
  if (pos == std::string::npos) {
    return "";
  }
  return tmp.substr(pos + 1);
}

int File::Create(const std::string &path) {
  if (Exist(path)) {
    return kSuccess;
  }

  auto parent_path = std::filesystem::path(path).parent_path();
  if (parent_path != "" && !std::filesystem::exists(parent_path)) {
    std::filesystem::create_directories(
        std::filesystem::path(path).parent_path());
  }

  std::ofstream ofs(path, std::ios::out | std::ios::binary);
  if (!ofs.is_open()) {
    return kSysErr;
  }
  ofs.close();
  return kSuccess;
}

int File::Remove(const std::string &path) {
  if (!Exist(path)) {
    return kSuccess;
  }
  return std::filesystem::remove(path) ? kSuccess : kSysErr;
}

int File::Append(const std::string &path, const std::string &data) {
  std::ofstream ofs(path, std::ios::out | std::ios::binary | std::ios::app);
  if (!ofs.is_open()) {
    return kLogicErr;
  }
  ofs << data;
  ofs.close();
  return kSuccess;
}

bool File::CanWrite(const std::string &path) {
  std::filesystem::file_status status = std::filesystem::status(path);
  return (status.permissions() & std::filesystem::perms::owner_write) !=
         std::filesystem::perms::none;
}

} // namespace cppnet
