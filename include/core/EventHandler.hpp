#pragma once

#include "Event.hpp"

namespace vq::core {

/*----------------------------------------------------------------------------*/

class EventHandler {
  public:
    EventHandler(void)                = default;
    virtual ~EventHandler(void)       = default;
    virtual void handle(Event& event) = 0;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
