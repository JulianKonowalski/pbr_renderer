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

    EventBase(EventBase&& other)      = delete;
    EventBase(const EventBase& other) = delete;

    EventBase& operator=(EventBase&& other)      = delete;
    EventBase& operator=(const EventBase& other) = delete;

    inline bool is_handled() const;
    inline void set_handled();

  private:
    bool m_is_handled;
};

/*----------------------------------------------------------------------------*/

template <typename EventType>
class EventHandlerBase {
    static_assert(std::is_base_of_v<EventBase, EventType>,
                  "EventType must derive from vq::core::EventBase");

    friend EventEmitterBase<EventType>;

    using ThisType = EventHandlerBase<EventType>;

  public:
    EventHandlerBase() = default;
    ~EventHandlerBase();

    EventHandlerBase(ThisType&& other);
    EventHandlerBase(const ThisType& other);

    ThisType& operator=(ThisType&& other);
    ThisType& operator=(const ThisType& other);

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

    using ThisType = EventEmitterBase<EventType>;

  public:
    EventEmitterBase() = default;
    ~EventEmitterBase();

    EventEmitterBase(ThisType&& other);
    EventEmitterBase(const ThisType& other);

    ThisType& operator=(ThisType&& other);
    ThisType& operator=(const ThisType& other);

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

    using ThisType = EventEmitter<EventTypes...>;

  public:
    EventEmitter()  = default;
    ~EventEmitter() = default;

    EventEmitter(ThisType&& other);
    EventEmitter(const ThisType& other);

    ThisType& operator=(ThisType&& other);
    ThisType& operator=(const ThisType& other);

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
class EventHandler : public EventHandlerBase<EventTypes>... {

    using ThisType = EventHandler<EventTypes...>;

  public:
    EventHandler()  = default;
    ~EventHandler() = default;

    EventHandler(ThisType&& other);
    EventHandler(const ThisType& other);

    ThisType& operator=(ThisType&& other);
    ThisType& operator=(const ThisType& other);
};

/*----------------------------------------------------------------------------*/

inline bool EventBase::is_handled() const { return m_is_handled; }

inline void EventBase::set_handled() { m_is_handled = true; }

/*----------------------------------------------------------------------------*/

template <typename EventType>
EventHandlerBase<EventType>::~EventHandlerBase() {
    for (auto* event_emitter : m_event_emitters) {
        event_emitter->detach_event_handler(*this);
    }
}

template <typename EventType>
EventHandlerBase<EventType>::EventHandlerBase(ThisType&& other) {
    *this = std::move(other);
}

template <typename EventType>
EventHandlerBase<EventType>::EventHandlerBase(const ThisType& other) {
    *this = other;
}

template <typename EventType>
EventHandlerBase<EventType>&
EventHandlerBase<EventType>::operator=(ThisType&& other) {
    for (auto* event_emitter : other.m_event_emitters) {
        event_emitter->detach_event_handler(other);
        event_emitter->attach_event_handler(*this);
    }
    return *this;
}

template <typename EventType>
EventHandlerBase<EventType>&
EventHandlerBase<EventType>::operator=(const ThisType& other) {
    for (auto* event_emitter : other.m_event_emitters) {
        event_emitter->attach_event_handler(*this);
    }
    return *this;
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
EventEmitterBase<EventType>::EventEmitterBase(ThisType&& other) {
    *this = std::move(other);
}

template <typename EventType>
EventEmitterBase<EventType>::EventEmitterBase(const ThisType& other) {
    *this = other;
}

template <typename EventType>
EventEmitterBase<EventType>&
EventEmitterBase<EventType>::operator=(ThisType&& other) {
    for (auto* event_handler : other.m_event_handlers) {
        this->attach_event_handler(*event_handler);
        other.detach_event_handler(*event_handler);
    }
    return *this;
}

template <typename EventType>
EventEmitterBase<EventType>&
EventEmitterBase<EventType>::operator=(const ThisType& other) {
    for (auto* event_handler : other.m_event_handlers) {
        this->attach_event_handler(*event_handler);
    }
    return *this;
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
EventEmitter<EventTypes...>::EventEmitter(ThisType&& other)
    : EventEmitterBase<EventTypes>(std::move(other))... {}

template <typename... EventTypes>
EventEmitter<EventTypes...>::EventEmitter(const ThisType& other)
    : EventEmitterBase<EventTypes>(other)... {}

template <typename... EventTypes>
EventEmitter<EventTypes...>&
EventEmitter<EventTypes...>::operator=(ThisType&& other) {
    (EventEmitterBase<EventTypes>::operator=(std::move(other)), ...);
    return *this;
}

template <typename... EventTypes>
EventEmitter<EventTypes...>&
EventEmitter<EventTypes...>::operator=(const ThisType& other) {
    (EventEmitterBase<EventTypes>::operator=(other), ...);
    return *this;
}

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

template <typename... EventTypes>
EventHandler<EventTypes...>::EventHandler(ThisType&& other)
    : EventHandlerBase<EventTypes>(std::move(other))... {}

template <typename... EventTypes>
EventHandler<EventTypes...>::EventHandler(const ThisType& other)
    : EventHandlerBase<EventTypes>(other)... {}

template <typename... EventTypes>
EventHandler<EventTypes...>&
EventHandler<EventTypes...>::operator=(ThisType&& other) {
    (EventHandlerBase<EventTypes>::operator=(std::move(other)), ...);
    return *this;
}

template <typename... EventTypes>
EventHandler<EventTypes...>&
EventHandler<EventTypes...>::operator=(const ThisType& other) {
    (EventHandlerBase<EventTypes>::operator=(other), ...);
    return *this;
}

/*----------------------------------------------------------------------------*/

} // namespace vq::core
