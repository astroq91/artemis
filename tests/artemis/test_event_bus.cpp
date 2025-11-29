#include <gtest/gtest.h>
#include <artemis/events/event.hpp>
#include <artemis/events/event_bus.hpp>

using namespace artemis;

struct TestEvent : Event {
    std::string str;
    int a;
};

TEST(artemis, test_publish) {
    TestEvent event;
    event.str = "string!";
    event.a = 64;

    EventBus bus;
    bus.subscribe<TestEvent>([&](const TestEvent& e) {
        ASSERT_EQ("string!", e.str);
        ASSERT_EQ(64, e.a);
    });
    bus.publish(event);
}
