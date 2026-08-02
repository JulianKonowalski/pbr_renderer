#include "core/EventEmitter.hpp"

#include "core/Event.hpp"
#include "core/EventHandler.hpp"

#include <gtest/gtest.h>

namespace vq::tests::core::EventEmitter {

/*----------------------------------------------------------------------------*/

class MockEvent : public vq::core::Event {};

class MockEventEmitter : public vq::core::EventEmitter {
  public:
    using vq::core::EventEmitter::EventEmitter;

    void emit_mock_event() {
        MockEvent mock_event;
        this->emit_event(mock_event);
    }
};

class MockEventHandler : public vq::core::EventHandler {
  public:
    MockEventHandler() : m_event_handled(false) {}
    ~MockEventHandler() override = default;

    inline bool get_event_handled() const { return m_event_handled; }

    void handle(vq::core::Event& event) override {
        if (auto* p_casted = dynamic_cast<MockEvent*>(&event)) {
            m_event_handled = true;
            p_casted->set_handled();
        }
    }

  private:
    bool m_event_handled;
};

/*----------------------------------------------------------------------------*/

TEST(Core_EventEmitter, handler_is_attached_and_detached) {
    MockEventEmitter event_emitter;
    MockEventHandler event_handler;

    ASSERT_EQ(event_emitter.get_event_handler_count(), 0);
    ASSERT_FALSE(event_handler.get_event_handled());

    event_emitter.attach_event_handler(event_handler);
    ASSERT_FALSE(event_handler.get_event_handled());
    ASSERT_EQ(event_emitter.get_event_handler_count(), 1);

    event_emitter.detach_event_handler(event_handler);
    event_emitter.emit_mock_event();
    ASSERT_FALSE(event_handler.get_event_handled());
    ASSERT_EQ(event_emitter.get_event_handler_count(), 0);
}

/*----------------------------------------------------------------------------*/

TEST(Core_EventEmitter, event_is_emitted) {
    MockEventEmitter event_emitter;
    MockEventHandler event_handler;

    event_emitter.attach_event_handler(event_handler);
    ASSERT_FALSE(event_handler.get_event_handled());

    event_emitter.emit_mock_event();
    ASSERT_TRUE(event_handler.get_event_handled());
}

/*----------------------------------------------------------------------------*/

TEST(Core_EventEmitter, event_loop_breaks_on_event_handled) {
    MockEventEmitter event_emitter;
    MockEventHandler event_handler_1;
    MockEventHandler event_handler_2;

    event_emitter.attach_event_handler(event_handler_1);
    event_emitter.attach_event_handler(event_handler_2);
    ASSERT_FALSE(event_handler_1.get_event_handled());
    ASSERT_FALSE(event_handler_2.get_event_handled());

    event_emitter.emit_mock_event();
    ASSERT_TRUE(event_handler_1.get_event_handled());
    ASSERT_FALSE(event_handler_2.get_event_handled());
}

/*----------------------------------------------------------------------------*/

} // namespace vq::tests::core::EventEmitter
