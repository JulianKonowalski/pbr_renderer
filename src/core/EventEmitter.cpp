#include "core/EventEmitter.hpp"

using namespace vq::core;

/*----------------------------------------------------------------------------*/

void EventEmitter::detach_handler(EventHandler& event_handler) {
    auto event_handler_it =
        std::find_if(m_event_handlers.begin(), m_event_handlers.end(),
                     [&](auto* handler) { return handler == &event_handler; });
    if (event_handler_it != m_event_handlers.end()) {
        m_event_handlers.erase(event_handler_it);
    }
}

/*----------------------------------------------------------------------------*/

void EventEmitter::emit_event(Event& event) {
    for (auto* event_handler : m_event_handlers) {
        event_handler->handle(event);
        if (event.is_handled()) {
            break;
        }
    }
}

/*----------------------------------------------------------------------------*/
