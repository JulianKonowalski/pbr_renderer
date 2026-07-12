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

    void attach_handler(EventHandler& event_handler) {
        m_event_handlers.push_back(&event_handler);
    }

    void detach_handler(EventHandler& event_handler) {
        auto event_handler_it = std::find_if(
            m_event_handlers.begin(), m_event_handlers.end(),
            [&](auto* handler) { return handler == &event_handler; });
        if (event_handler_it != m_event_handlers.end()) {
            m_event_handlers.erase(event_handler_it);
        }
    }

  protected:
    void emit_event(Event& event) {
        for (auto* event_handler : m_event_handlers) {
            event_handler->handle(event);
            if (event.is_handled()) {
                break;
            }
        }
    }

  private:
    std::vector<EventHandler*> m_event_handlers;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
