#pragma once

#include "Event.hpp"
#include "EventHandler.hpp"

#include <vector>

namespace vq::core {

/*----------------------------------------------------------------------------*/

class EventEmitter {
  public:
    EventEmitter()  = default;
    ~EventEmitter() = default;

    inline size_t get_event_handler_count() const {
        return m_event_handlers.size();
    }

    inline void attach_event_handler(EventHandler& event_handler) {
        m_event_handlers.push_back(&event_handler);
    }

    void detach_event_handler(EventHandler& event_handler);

  protected:
    void emit_event(Event& event);

  private:
    std::vector<EventHandler*> m_event_handlers;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
