#include "core/Event.hpp"

using namespace pbr::core;

/*----------------------------------------------------------------------------*/

EventHandler::EventHandler(EventLoop& event_loop) : m_event_loop(event_loop) {
    m_event_loop.attach_handler(this);
}

/*----------------------------------------------------------------------------*/

EventHandler::~EventHandler(void) { m_event_loop.detach_handler(this); }

/*----------------------------------------------------------------------------*/

void EventDispatcher::dispatch_event(Event& event) {
    m_event_loop.dispatch_event(event);
}

/*----------------------------------------------------------------------------*/