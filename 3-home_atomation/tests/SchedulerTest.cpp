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
// Remove non-scheduled light schedule
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
    SchedulerTest() : time_service_mock(std::make_shared<TimeServiceMock>()),
                      light_controller_mock(std::make_shared<LightControllerMock>()),
                      scheduler(std::make_shared<Scheduler>(time_service_mock, light_controller_mock)) {}
    std::shared_ptr<TimeServiceMock> time_service_mock;
    std::shared_ptr<LightControllerMock> light_controller_mock;
    std::shared_ptr<Scheduler> scheduler;
};

TEST_F(SchedulerTest, ScheduleEvent) {
    const uint8_t kLightId = 1;

    scheduler->addEvent(kLightId, ITimeService::Day::kMonday, 1200, ILightController::State::kOn);

    Scheduler::Event expected_event{kLightId,
                                    {ITimeService::Day::kMonday, 1200},
                                    ILightController::State::kOn};
    EXPECT_EQ(scheduler->getLastAddedEvent().light_state, expected_event.light_state);
    EXPECT_EQ(scheduler->getLastAddedEvent().time.day, expected_event.time.day);
    EXPECT_EQ(scheduler->getLastAddedEvent().time.minute, expected_event.time.minute);
    EXPECT_EQ(scheduler->getLastAddedEvent().light_state, expected_event.light_state);
}

TEST_F(SchedulerTest, ScheduleTwoEvents) {
    const uint8_t kLightId = 1;

    scheduler->addEvent(kLightId, ITimeService::Day::kMonday, 1200, ILightController::State::kOn);

    Scheduler::Event expected_event1{kLightId,
                                    {ITimeService::Day::kMonday, 1200},
                                    ILightController::State::kOn};
    EXPECT_EQ(scheduler->getLastAddedEvent().light_state, expected_event1.light_state);
    EXPECT_EQ(scheduler->getLastAddedEvent().time.day, expected_event1.time.day);
    EXPECT_EQ(scheduler->getLastAddedEvent().time.minute, expected_event1.time.minute);
    EXPECT_EQ(scheduler->getLastAddedEvent().light_state, expected_event1.light_state);

    scheduler->addEvent(kLightId, ITimeService::Day::kTuesday, 1400, ILightController::State::kOff);
    Scheduler::Event expected_event2{kLightId,
                                    {ITimeService::Day::kTuesday, 1400},
                                    ILightController::State::kOff};
    EXPECT_EQ(scheduler->getLastAddedEvent().light_state, expected_event2.light_state);
    EXPECT_EQ(scheduler->getLastAddedEvent().time.day, expected_event2.time.day);
    EXPECT_EQ(scheduler->getLastAddedEvent().time.minute, expected_event2.time.minute);
    EXPECT_EQ(scheduler->getLastAddedEvent().light_state, expected_event2.light_state);
}

TEST_F(SchedulerTest, ScheduleAndTriggerOn) {
    const uint8_t kLightId = 1;

    scheduler->addEvent(kLightId, ITimeService::Day::kMonday, 1200, ILightController::State::kOn);

    EXPECT_CALL(*time_service_mock, getTime())
            .WillOnce(testing::Return(ITimeService::Time{ITimeService::Day::kMonday, 1200}));
    EXPECT_CALL(*light_controller_mock, turnOn(kLightId));
    scheduler->triggerEvent();
}

TEST_F(SchedulerTest, ScheduleAndTriggerOff) {
    const uint8_t kLightId = 1;

    scheduler->addEvent(kLightId, ITimeService::Day::kMonday, 1200, ILightController::State::kOff);

    EXPECT_CALL(*time_service_mock, getTime())
            .WillOnce(testing::Return(ITimeService::Time{ITimeService::Day::kMonday, 1200}));
    EXPECT_CALL(*light_controller_mock, turnOff(kLightId));
    scheduler->triggerEvent();
}


