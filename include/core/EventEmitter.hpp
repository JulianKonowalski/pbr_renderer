#pragma once

#include "Event.hpp"
#include "EventHandler.hpp"

#include <vector>

namespace vq::core {

/*----------------------------------------------------------------------------*/

class EventEmitter {
  public:
    EventEmitter(void)  = default;
    ~EventEmitter(void) = default;

    void detach_handler(EventHandler& event_handler);
    inline void attach_handler(EventHandler& event_handler) {
        m_event_handlers.push_back(&event_handler);
    }

  protected:
    void emit_event(Event& event);

  private:
    std::vector<EventHandler*> m_event_handlers;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
