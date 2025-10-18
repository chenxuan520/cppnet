#include "date.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
namespace cppnet {

std::string Date::GetNow(std::string format) {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm local_tm;
#ifdef _WIN32
  localtime_s(&local_tm, &now_time);
#else
  localtime_r(&now_time, &local_tm);
#endif

  std::stringstream ss;
  ss << std::put_time(&local_tm, format.c_str());
  return ss.str();
}

} // namespace cppnet
