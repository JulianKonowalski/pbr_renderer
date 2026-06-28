#pragma once

#include <iostream>
#include <type_traits>

namespace pbr::core {

/*----------------------------------------------------------------------------*/

struct Event {
  public:
    Event(void) : m_is_handled(false) {}
    ~Event(void) = default;

    inline bool is_handled(void) const { return m_is_handled; }
    inline void set_handled(void) { m_is_handled = true; }

  private:
    bool m_is_handled;
};

/*----------------------------------------------------------------------------*/

class EventLoop {

    template <typename... Events>
    friend class EventDispatcher;

    template <typename EventHandlerType, typename... Events>
    friend class EventHandler;

  public:
    EventLoop(void)  = default;
    ~EventLoop(void) = default;

  protected:
    template <typename EventType>
    void dispatch_event(void) {
        static_assert(std::is_base_of_v<Event, EventType>,
                      "Attached event should be a child of pbr::core::Event");
    }

    template <typename EventType>
    void attach_handler(void) {
        static_assert(std::is_base_of_v<Event, EventType>,
                      "Attached event should be a child of pbr::core::Event");
        // todo
    }

    void detach_handler(void) {
        // todo
    }
};

/*----------------------------------------------------------------------------*/

template <typename... Events>
class EventDispatcher {
  public:
    EventDispatcher(EventLoop& event_loop) : m_event_loop(event_loop) {}
    ~EventDispatcher(void) = default;

  protected:
    template <typename EventType, typename... Args>
    void dispatch_event(Args&&... args) {
        static_assert(std::is_base_of_v<Event, EventType>,
                      "Dispatched event must be a child of pbr::core::Event");
        static_assert((std::is_same_v<EventType, Events> || ...),
                      "Dispatched event must be one of the allowed events");
    }

  private:
    EventLoop& m_event_loop;
};

/*----------------------------------------------------------------------------*/

template <typename EventHandlerType, typename... Events>
class EventHandler {
  public:
    EventHandler(EventHandlerType& event_handler, EventLoop& event_loop)
        : m_event_loop(event_loop) {
        static_assert(
            std::is_base_of_v<EventHandler<EventHandlerType, Events...>,
                              EventHandlerType>,
            "EventHandlerType should be a child of pbr::core::EventHandler");
        // static_assert(is_call_, ); TODO: check if call is possible
        (m_event_loop.attach_handler<Events>(), ...);
    }
    ~EventHandler(void) {
        // detach child methods
    }

  private:
    EventLoop& m_event_loop;
};

/*----------------------------------------------------------------------------*/

} // namespace pbr::core