#pragma once

#include "Event.hpp"

namespace vq::core {

/*----------------------------------------------------------------------------*/

class EventHandler {
  public:
    EventHandler()                    = default;
    virtual ~EventHandler()           = default;
    virtual void handle(Event& event) = 0;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
