#include "file.hpp"
#include "utils/const.hpp"

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
  std::ifstream file(path);
  return file.good();
}

} // namespace cppnet
