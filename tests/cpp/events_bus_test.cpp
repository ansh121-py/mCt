#include <gtest/gtest.h>

#include <future>
#include <stdexcept>

#include "events/AsyncEventBus.hpp"
#include "events/EventBus.hpp"

// Two distinct event types, so we can test that they stay separate.
struct PingEvent {
    int value = 0;
};

struct PongEvent {
    int value = 0;
};

TEST(EventBusTest, DeliversPublishedEventToSubscriber) {
    mct::events::EventBus bus;
    int received = 0;

    bus.subscribeTo<PingEvent>([&received](const PingEvent& e) {
        received = e.value;
    });

    bus.publish(PingEvent{42});

    EXPECT_EQ(received, 42);
}

TEST(EventBusTest, UnsubscribeStopsDelivery) {
    mct::events::EventBus bus;
    int callCount = 0;

    auto id = bus.subscribeTo<PingEvent>([&callCount](const PingEvent&) {
        ++callCount;
    });

    bus.publish(PingEvent{1});
    bus.unsubscribe(id);
    bus.publish(PingEvent{2});

    EXPECT_EQ(callCount, 1);
}

TEST(EventBusTest, AllSubscribersReceiveEvent) {
    mct::events::EventBus bus;
    int first = 0;
    int second = 0;

    bus.subscribeTo<PingEvent>([&first](const PingEvent& e) { first = e.value; });
    bus.subscribeTo<PingEvent>([&second](const PingEvent& e) { second = e.value; });

    bus.publish(PingEvent{7});

    EXPECT_EQ(first, 7);
    EXPECT_EQ(second, 7);
}

TEST(EventBusTest, SubscribersOnlyReceiveTheirEventType) {
    mct::events::EventBus bus;
    int pingCount = 0;

    bus.subscribeTo<PingEvent>([&pingCount](const PingEvent&) { ++pingCount; });

    bus.publish(PongEvent{1});

    EXPECT_EQ(pingCount, 0);
}

TEST(EventBusTest, ThrowingSubscriberDoesNotStopOthers) {
    mct::events::EventBus bus;
    int secondCalled = 0;

    bus.subscribeTo<PingEvent>([](const PingEvent&) {
        throw std::runtime_error("boom");
    });
    bus.subscribeTo<PingEvent>([&secondCalled](const PingEvent&) {
        ++secondCalled;
    });

    bus.publish(PingEvent{1});

    EXPECT_EQ(secondCalled, 1);
}

TEST(EventBusTest, UnsubscribeUnknownIdIsIgnored) {
    mct::events::EventBus bus;

    // Should simply do nothing, not crash.
    bus.unsubscribe(999);

    EXPECT_EQ(bus.subscriberCount<PingEvent>(), 0u);
}

TEST(EventBusTest, SubscriberCountTracksSubscribeAndUnsubscribe) {
    mct::events::EventBus bus;

    auto first = bus.subscribeTo<PingEvent>([](const PingEvent&) {});
    bus.subscribeTo<PingEvent>([](const PingEvent&) {});
    EXPECT_EQ(bus.subscriberCount<PingEvent>(), 2u);

    bus.unsubscribe(first);
    EXPECT_EQ(bus.subscriberCount<PingEvent>(), 1u);
}

TEST(AsyncEventBusTest, DeliversEventOnWorkerThread) {
    mct::events::AsyncEventBus bus;
    std::promise<int> result;

    bus.subscribeTo<PingEvent>([&result](const PingEvent& e) {
        result.set_value(e.value);
    });

    bus.publish(PingEvent{5});

    EXPECT_EQ(result.get_future().get(), 5);
}

TEST(AsyncEventBusTest, ShutdownDeliversQueuedEvents) {
    int count = 0;
    {
        mct::events::AsyncEventBus bus;
        bus.subscribeTo<PingEvent>([&count](const PingEvent&) { ++count; });

        for (int i = 0; i < 100; ++i) {
            bus.publish(PingEvent{i});
        }
        // Destructor calls shutdown(), which must drain the queue.
    }

    EXPECT_EQ(count, 100);
}