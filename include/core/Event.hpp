#pragma once

#include <type_traits>
#include <vector>

namespace vq::core {

/*----------------------------------------------------------------------------*/

template <typename EventType>
class EventEmitterBase;

/*----------------------------------------------------------------------------*/

class EventBase {
  public:
    EventBase() : m_is_handled(false) {}
    ~EventBase() = default;

    inline bool is_handled() const { return m_is_handled; }
    inline void set_handled() { m_is_handled = true; }

  private:
    bool m_is_handled;
};

/*----------------------------------------------------------------------------*/

template <typename EventType>
class EventHandlerBase {
    static_assert(std::is_base_of_v<EventBase, EventType>,
                  "EventType must derive from vq::core::EventBase");

    friend EventEmitterBase<EventType>;

  public:
    EventHandlerBase() = default;
    ~EventHandlerBase();

  protected:
    virtual void handle(EventType& event) noexcept {}

  private:
    bool attach_event_emitter(EventEmitterBase<EventType>& event_emitter);
    bool detach_event_emitter(EventEmitterBase<EventType>& event_emitter);

    std::vector<EventEmitterBase<EventType>*> m_event_emitters;
};

/*----------------------------------------------------------------------------*/

template <typename EventType>
class EventEmitterBase {
    static_assert(std::is_base_of_v<EventBase, EventType>,
                  "EventType must derive from vq::core::EventBase");

  public:
    EventEmitterBase() = default;
    ~EventEmitterBase();

    bool attach_event_handler(EventHandlerBase<EventType>& event_handler);
    bool detach_event_handler(EventHandlerBase<EventType>& event_handler);

  protected:
    void emit_event(EventType& event);

  private:
    std::vector<EventHandlerBase<EventType>*> m_event_handlers;
};

/*----------------------------------------------------------------------------*/

template <typename... EventTypes>
class EventEmitter : private EventEmitterBase<EventTypes>... {
  public:
    template <typename EventType>
    bool attach_event_handler(EventHandlerBase<EventType>& event_handler);

    template <typename EventType>
    bool detach_event_handler(EventHandlerBase<EventType>& event_handler);

  protected:
    template <typename EventType, typename... Args>
    void emit_event(Args&&... args);
};

/*----------------------------------------------------------------------------*/

template <typename... EventTypes>
class EventHandler : public EventHandlerBase<EventTypes>... {};

/*----------------------------------------------------------------------------*/

template <typename EventType>
EventHandlerBase<EventType>::~EventHandlerBase() {
    for (auto* event_emitter : m_event_emitters) {
        event_emitter->detach_event_handler(*this);
    }
}

template <typename EventType>
bool EventHandlerBase<EventType>::attach_event_emitter(
    EventEmitterBase<EventType>& event_emitter) {
    auto it =
        std::find_if(m_event_emitters.begin(), m_event_emitters.end(),
                     [&](auto* emitter) { return emitter == &event_emitter; });
    if (it != m_event_emitters.end()) {
        return false;
    }

    m_event_emitters.push_back(&event_emitter);
    return true;
}

template <typename EventType>
bool EventHandlerBase<EventType>::detach_event_emitter(
    EventEmitterBase<EventType>& event_emitter) {
    auto it =
        std::find_if(m_event_emitters.begin(), m_event_emitters.end(),
                     [&](auto* emitter) { return emitter == &event_emitter; });
    if (it == m_event_emitters.end()) {
        return false;
    }

    m_event_emitters.erase(it);
    return true;
}

/*----------------------------------------------------------------------------*/

template <typename EventType>
EventEmitterBase<EventType>::~EventEmitterBase() {
    for (auto* event_handler : m_event_handlers) {
        event_handler->detach_event_emitter(*this);
    }
}

template <typename EventType>
bool EventEmitterBase<EventType>::attach_event_handler(
    EventHandlerBase<EventType>& event_handler) {
    auto it =
        std::find_if(m_event_handlers.begin(), m_event_handlers.end(),
                     [&](auto* handler) { return handler == &event_handler; });
    if (it != m_event_handlers.end()) {
        return false;
    }
    m_event_handlers.push_back(&event_handler);
    return true;
}

template <typename EventType>
bool EventEmitterBase<EventType>::detach_event_handler(
    EventHandlerBase<EventType>& event_handler) {
    auto it =
        std::find_if(m_event_handlers.begin(), m_event_handlers.end(),
                     [&](auto* handler) { return handler == &event_handler; });
    if (it == m_event_handlers.end()) {
        return false;
    }
    m_event_handlers.erase(it);
    return true;
}

template <typename EventType>
void EventEmitterBase<EventType>::emit_event(EventType& event) {
    for (auto* event_handler : m_event_handlers) {
        event_handler->handle(event);
        if (event.is_handled()) {
            break;
        }
    }
}

/*----------------------------------------------------------------------------*/

template <typename... EventTypes>
template <typename EventType>
bool EventEmitter<EventTypes...>::attach_event_handler(
    EventHandlerBase<EventType>& event_handler) {
    static_assert((std::is_same_v<EventType, EventTypes> || ...),
                  "EventType must be one of the allowed EventTypes");
    return EventEmitterBase<EventType>::attach_event_handler(event_handler);
}

template <typename... EventTypes>
template <typename EventType>
bool EventEmitter<EventTypes...>::detach_event_handler(
    EventHandlerBase<EventType>& event_handler) {
    static_assert((std::is_same_v<EventType, EventTypes> || ...),
                  "EventType must be one of the allowed EventTypes");
    return EventEmitterBase<EventType>::detach_event_handler(event_handler);
}

template <typename... EventTypes>
template <typename EventType, typename... Args>
void EventEmitter<EventTypes...>::emit_event(Args&&... args) {
    static_assert((std::is_same_v<EventType, EventTypes> || ...),
                  "EventType must be one of the allowed EventTypes");
    EventType event(args...);
    EventEmitterBase<EventType>::emit_event(event);
}

/*----------------------------------------------------------------------------*/

} // namespace vq::core
