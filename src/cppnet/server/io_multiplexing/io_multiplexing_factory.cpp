#include "io_multiplexing_factory.hpp"
#include "epoll.hpp"

namespace cppnet {

std::shared_ptr<IOMultiplexingBase>
IOMultiplexingFactory::Create(const IOMultiplexingType type) {
  switch (type) {
  case IOMultiplexingType::kEpoll:
    return std::make_shared<Epoll>();
  default:
    return nullptr;
  }
}

} // namespace cppnet
