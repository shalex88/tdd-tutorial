#include <ostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
// Add your project include files here
#include "../source/Scheduler.h"

// TESTS LIST:
// Lights are not changed at initialization
// Time is wrong, day is wrong, no lights are changed
// Day is right, time is wrong, no lights are changed
// Day is wrong, time is right, no lights are changed
// Day is right, time is right, the right light is turned on
// Day is right, time is right, the right light is turned off
// Schedule every day
// Schedule a specific day
// Schedule all weekdays
// Schedule weekend days
// Remove scheduled event
// Remove non-existent event
// Multiple scheduled events at the same time
// Multiple scheduled events for the same light
// Remove non scheduled light schedule
// Schedule the maximum supported number of events (128)
// Schedule too many events

class TimeServiceMock : public ITimeService {
public:
    MOCK_METHOD0(getTime, Time());
};

class LightControllerMock : public ILightController {
public:
    MOCK_METHOD1(turnOn, void(uint8_t light_id));
    MOCK_METHOD1(turnOff, void(uint8_t light_id));
    MOCK_METHOD1(getStatus, bool(uint8_t light_id));
};

class SchedulerTest : public testing::Test {
public:
    SchedulerTest() {
        scheduler = new Scheduler(&time_service_mock, &light_controller_mock);
    }
    TimeServiceMock time_service_mock;
    LightControllerMock light_controller_mock;
    Scheduler *scheduler {};
    const uint8_t kLightId = 1;
};

TEST_F(SchedulerTest, ScheduleEvent) {
    scheduler->addEvent(kLightId, ITimeService::Day::kMonday, 1200, ILightController::State::kOn);

    Scheduler::Event expected_event{kLightId,
                                    {ITimeService::Day::kMonday, 1200},
                                    ILightController::State::kOn};
    EXPECT_EQ(scheduler->getNextEvent().light_state, expected_event.light_state);
    EXPECT_EQ(scheduler->getNextEvent().time.day, expected_event.time.day);
    EXPECT_EQ(scheduler->getNextEvent().time.minute, expected_event.time.minute);
    EXPECT_EQ(scheduler->getNextEvent().light_state, expected_event.light_state);
}

TEST_F(SchedulerTest, ScheduleAndTriggerOn) {
    scheduler->addEvent(kLightId, ITimeService::Day::kMonday, 1200, ILightController::State::kOn);

    EXPECT_CALL(time_service_mock, getTime())
            .WillOnce(testing::Return(ITimeService::Time{ITimeService::Day::kMonday, 1200}));
    EXPECT_CALL(light_controller_mock, turnOn(kLightId));
    scheduler->triggerEvent();
}

TEST_F(SchedulerTest, ScheduleAndTriggerOff) {
    scheduler->addEvent(kLightId, ITimeService::Day::kMonday, 1200, ILightController::State::kOff);

    EXPECT_CALL(time_service_mock, getTime())
            .WillOnce(testing::Return(ITimeService::Time{ITimeService::Day::kMonday, 1200}));
    EXPECT_CALL(light_controller_mock, turnOff(kLightId));
    scheduler->triggerEvent();
}


