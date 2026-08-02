#include "core/Event.hpp"

#include <gtest/gtest.h>

namespace vq::tests::core::Event {

TEST(Core_Event, event_is_handled) {
    vq::core::Event event;
    ASSERT_FALSE(event.is_handled());
    event.set_handled();
    ASSERT_TRUE(event.is_handled());
}

} // namespace vq::tests::core::Event