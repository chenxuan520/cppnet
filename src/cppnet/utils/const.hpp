#pragma once

namespace cppnet {
/**
 * @brief: Return code enum.
 */
enum RC {
  kSuccess = 0,
  kSysErr = -1,
  kLogicErr = -2,
  kNotSupport = -3,
};

} // namespace cppnet
