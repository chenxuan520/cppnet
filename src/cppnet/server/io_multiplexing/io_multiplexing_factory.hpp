#pragma once

#include "io_multiplexing_base.hpp"
#include <memory>
namespace cppnet {

class IOMultiplexingFactory {
public:
  /*
   * @brief: Create IOMultiplexingBase.
   */
  static std::shared_ptr<IOMultiplexingBase>
  Create(const IOMultiplexingType type);
};

} // namespace cppnet
