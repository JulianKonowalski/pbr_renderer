#pragma once

#include <type_traits>

namespace pbr::core {

/*----------------------------------------------------------------------------*/

struct EventBase {
  public:
    EventBase(void) : m_is_handled(false) {}
    ~EventBase(void) = default;

    inline bool is_handled(void) const { return m_is_handled; }
    inline void set_handled(void) { m_is_handled = true; }

  private:
    bool m_is_handled;
};

/*----------------------------------------------------------------------------*/

class EventLoopBase {
  public:
    EventLoopBase(void)  = default;
    ~EventLoopBase(void) = default;

  protected:
  private:
};

/*----------------------------------------------------------------------------*/

template <typename EventLoopType>
class EventDispatcher {
  public:
    EventDispatcher(EventLoopType& event_loop) : m_event_loop(event_loop) {}
    ~EventDispatcher(void) = default;

  protected:
    template <typename EventType>
    void dispatch_event(const EventType& event) {
        static_assert(std::is_base_of<EventBase, EventType>::value,
                      "Dispatched event must be a child of pbr::core::Event");
        //
    }

  private:
    EventLoopType& m_event_loop;
};

/*----------------------------------------------------------------------------*/

class EventListener {
  public:
    EventListener(void)  = default;
    ~EventListener(void) = default;

  private:
};

/*----------------------------------------------------------------------------*/

} // namespace pbr::core