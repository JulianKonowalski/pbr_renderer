#pragma once

#include <vector>

namespace pbr::core {

class EventLoop;

/*----------------------------------------------------------------------------*/

struct Event {
  public:
    Event(void) : m_is_handled(false) {}
    virtual ~Event(void) = default;

    inline bool is_handled(void) const { return m_is_handled; }
    inline void set_handled(void) { m_is_handled = true; }

  private:
    bool m_is_handled;
};

/*----------------------------------------------------------------------------*/

class EventHandler {
  public:
    explicit EventHandler(EventLoop& event_loop);
    ~EventHandler(void);
    virtual void handle(Event& event) = 0;

  private:
    EventLoop& m_event_loop;
};

/*----------------------------------------------------------------------------*/

class EventDispatcher {
  public:
    EventDispatcher(EventLoop& event_loop) : m_event_loop(event_loop) {}
    ~EventDispatcher(void) = default;

  protected:
    void dispatch_event(Event& event);

  private:
    EventLoop& m_event_loop;
};

/*----------------------------------------------------------------------------*/

class EventLoop {

    friend EventHandler;
    friend EventDispatcher;

  public:
    EventLoop(void)  = default;
    ~EventLoop(void) = default;

  protected:
    void dispatch_event(Event& event) {
        for (auto& handler : m_handlers) {
            handler->handle(event);
            if (event.is_handled()) {
                break;
            }
        }
    }

    void attach_handler(EventHandler* event_handler) {
        m_handlers.push_back(event_handler);
    }

    void detach_handler(EventHandler* event_handler) {
        auto res =
            std::find(m_handlers.begin(), m_handlers.end(), event_handler);
        if (res != m_handlers.end()) {
            m_handlers.erase(res);
        }
    }

  private:
    std::vector<EventHandler*> m_handlers;
};

/*----------------------------------------------------------------------------*/

} // namespace pbr::core