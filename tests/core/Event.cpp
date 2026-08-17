#include "core/Event.hpp"

#include <gtest/gtest.h>

namespace vq::tests::core::Event {

/*----------------------------------------------------------------------------*/

class MockEventWithTestValue : public vq::core::EventBase {
  public:
    explicit MockEventWithTestValue(const int test_value)
        : m_test_value(test_value) {}

    inline int get_test_value() const { return m_test_value; }

  private:
    const int m_test_value;
};

class MockEventEmitter : public vq::core::EventEmitter<MockEventWithTestValue> {
  public:
    inline void emit_mock_event() {
        this->emit_event<MockEventWithTestValue>(1);
    }
};

class MockEventHandler : public vq::core::EventHandler<MockEventWithTestValue> {
  public:
    MockEventHandler() : m_event_handled(false) {}

    void handle(MockEventWithTestValue& event) override {
        m_event_handled = true;
        event.set_handled();
    }

    inline bool event_handled() const { return m_event_handled; }
    inline void reset_event_handled() { m_event_handled = false; }

  private:
    bool m_event_handled;
};

/*----------------------------------------------------------------------------*/

TEST(Core_Event, event_is_handled) {
    vq::core::EventBase event;
    ASSERT_FALSE(event.is_handled());
    event.set_handled();
    ASSERT_TRUE(event.is_handled());
}

/*----------------------------------------------------------------------------*/

TEST(Core_Event, event_is_emitted) {
    MockEventEmitter event_emitter;
    MockEventHandler event_handler;
    event_emitter.attach_event_handler<MockEventWithTestValue>(event_handler);
    ASSERT_FALSE(event_handler.event_handled());
    event_emitter.emit_mock_event();
    ASSERT_TRUE(event_handler.event_handled());
}

/*----------------------------------------------------------------------------*/

TEST(Core_Event, handler_is_attached_and_detached) {
    MockEventEmitter event_emitter;
    MockEventHandler event_handler;

    event_emitter.attach_event_handler<MockEventWithTestValue>(event_handler);
    ASSERT_FALSE(event_handler.event_handled());

    event_emitter.emit_mock_event();
    ASSERT_TRUE(event_handler.event_handled());

    event_handler.reset_event_handled();
    event_emitter.detach_event_handler(event_handler);
    ASSERT_FALSE(event_handler.event_handled());

    event_emitter.emit_mock_event();
    ASSERT_FALSE(event_handler.event_handled());
}

/*----------------------------------------------------------------------------*/

TEST(Core_Event, event_loop_breaks_on_event_handled) {
    MockEventEmitter event_emitter;
    MockEventHandler event_handler_1;
    MockEventHandler event_handler_2;

    event_emitter.attach_event_handler(event_handler_1);
    event_emitter.attach_event_handler(event_handler_2);
    ASSERT_FALSE(event_handler_1.event_handled());
    ASSERT_FALSE(event_handler_2.event_handled());

    event_emitter.emit_mock_event();
    ASSERT_TRUE(event_handler_1.event_handled());
    ASSERT_FALSE(event_handler_2.event_handled());

    event_emitter.detach_event_handler(event_handler_1);
    event_handler_1.reset_event_handled();
    event_emitter.emit_mock_event();
    ASSERT_FALSE(event_handler_1.event_handled());
    ASSERT_TRUE(event_handler_2.event_handled());
}

/*----------------------------------------------------------------------------*/

TEST(Core_Event, handler_is_detached_upon_destruction) {
    MockEventEmitter event_emitter;

    {
        MockEventHandler event_handler;
        event_emitter.attach_event_handler(event_handler);
        ASSERT_FALSE(event_handler.event_handled());

        event_emitter.emit_mock_event();
        ASSERT_TRUE(event_handler.event_handled());
    }

    /** This will segfault if the handler destructor logic's flawed */
    event_emitter.emit_mock_event();
}

/*----------------------------------------------------------------------------*/

} // namespace vq::tests::core::Event