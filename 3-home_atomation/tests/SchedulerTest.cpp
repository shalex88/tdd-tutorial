#include "gtest/gtest.h"
#include "gmock/gmock.h"
// Add your project include files here
#include "../source/Scheduler.h"

// TESTS LIST:
// + Schedule every day
// + Schedule a specific day
// - Schedule all weekdays
// + Schedule multiple events
// + Schedule the maximum supported number of events (128)
// + Schedule too many events
// + Schedule multiple same events
// - Scheduled multiple events at the same time
// - Schedule multiple events for the same light
// + Remove scheduled event
// + Remove non-existent event
// - Lights are not changed at initialization
// - Time is wrong, day is wrong, no lights are changed
// - Day is right, time is wrong, no lights are changed
// - Day is wrong, time is right, no lights are changed
// - Day is right, time is right, the right light is turned on
// - Day is right, time is right, the right light is turned off

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

TEST_F(SchedulerTest, ScheduleEventEveryday) {
    uint8_t light_id = 1;

    scheduler->addEvent(light_id, ITimeService::Day::kEveryday, 1200, ILightController::State::kOn);

    Event expected_event{light_id,
                         {ITimeService::Day::kEveryday, 1200},
                         ILightController::State::kOn};
    auto event = scheduler->getLastAddedEvent();

    EXPECT_EQ(event.value().light_state, expected_event.light_state);
    EXPECT_EQ(event.value().time.day, expected_event.time.day);
    EXPECT_EQ(event.value().time.minute, expected_event.time.minute);
    EXPECT_EQ(event.value().light_state, expected_event.light_state);
}

TEST_F(SchedulerTest, ScheduleEvent) {
    uint8_t light_id = 1;

    scheduler->addEvent(light_id, ITimeService::Day::kMonday, 1200, ILightController::State::kOn);

    Event expected_event{light_id,
                         {ITimeService::Day::kMonday, 1200},
                         ILightController::State::kOn};
    auto event = scheduler->getLastAddedEvent();

    EXPECT_EQ(event.value().light_state, expected_event.light_state);
    EXPECT_EQ(event.value().time.day, expected_event.time.day);
    EXPECT_EQ(event.value().time.minute, expected_event.time.minute);
    EXPECT_EQ(event.value().light_state, expected_event.light_state);
}

TEST_F(SchedulerTest, UnscheduleEvent) {
    uint8_t light_id = 1;

    scheduler->addEvent(light_id, ITimeService::Day::kMonday, 1200, ILightController::State::kOn);
    auto event = scheduler->getLastAddedEvent();
    scheduler->removeEvent(event.value());
    event = scheduler->getLastAddedEvent();

    EXPECT_EQ(event.has_value(), false);
}

TEST_F(SchedulerTest, UnscheduleNotExistingEvent) {
    uint8_t light_id = 1;

    scheduler->addEvent(light_id, ITimeService::Day::kMonday, 1200, ILightController::State::kOn);
    Event expected_event1{light_id,
                          {ITimeService::Day::kTuesday, 1200},
                          ILightController::State::kOn};
    scheduler->removeEvent(expected_event1);
    auto event = scheduler->getLastAddedEvent();

    EXPECT_EQ(event.has_value(), true);
}

TEST_F(SchedulerTest, ScheduleTwoEvents) {
    uint8_t light_id = 1;

    scheduler->addEvent(light_id, ITimeService::Day::kMonday, 1200, ILightController::State::kOn);

    Event expected_event1{light_id,
                          {ITimeService::Day::kMonday, 1200},
                          ILightController::State::kOn};
    auto event = scheduler->getLastAddedEvent();

    EXPECT_EQ(event.value().light_state, expected_event1.light_state);
    EXPECT_EQ(event.value().time.day, expected_event1.time.day);
    EXPECT_EQ(event.value().time.minute, expected_event1.time.minute);
    EXPECT_EQ(event.value().light_state, expected_event1.light_state);

    light_id = 2;
    scheduler->addEvent(light_id, ITimeService::Day::kEveryday, 1400, ILightController::State::kOff);
    Event expected_event2{light_id,
                          {ITimeService::Day::kEveryday, 1400},
                          ILightController::State::kOff};
    event = scheduler->getLastAddedEvent();

    EXPECT_EQ(event.value().light_state, expected_event2.light_state);
    EXPECT_EQ(event.value().time.day, expected_event2.time.day);
    EXPECT_EQ(event.value().time.minute, expected_event2.time.minute);
    EXPECT_EQ(event.value().light_state, expected_event2.light_state);
}

TEST_F(SchedulerTest, ScheduleSameEvents) {
    // Save the original cerr buffer and redirect cerr to a custom stringstream
    std::streambuf *original_cerr_buffer = std::cerr.rdbuf();
    std::ostringstream capture_cerr;
    std::cerr.rdbuf(capture_cerr.rdbuf());

    const uint8_t kLightId = 1;

    for (uint32_t i{}; i < 2; ++i) {
        scheduler->addEvent(kLightId, ITimeService::Day::kMonday, 1200, ILightController::State::kOff);
    }

    // Restore the original cerr buffer
    std::cerr.rdbuf(original_cerr_buffer);

    // Compare the captured output with the expected output
    std::string expected_output{"ERROR: Events already exists"};
    EXPECT_THAT(capture_cerr.str(), ::testing::HasSubstr(expected_output));
}

TEST_F(SchedulerTest, ScheduleAndTriggerOn) {
    uint8_t light_id = 1;

    scheduler->addEvent(light_id, ITimeService::Day::kMonday, 1200, ILightController::State::kOn);

    EXPECT_CALL(*time_service_mock, getTime())
            .WillOnce(testing::Return(ITimeService::Time{ITimeService::Day::kMonday, 1200}));
    EXPECT_CALL(*light_controller_mock, turnOn(light_id));
    scheduler->triggerEvent();
}

TEST_F(SchedulerTest, ScheduleAndTriggerOff) {
    uint8_t light_id = 1;

    scheduler->addEvent(light_id, ITimeService::Day::kMonday, 1200, ILightController::State::kOff);

    EXPECT_CALL(*time_service_mock, getTime())
            .WillOnce(testing::Return(ITimeService::Time{ITimeService::Day::kMonday, 1200}));
    EXPECT_CALL(*light_controller_mock, turnOff(light_id));
    scheduler->triggerEvent();
}

TEST_F(SchedulerTest, ScheduleMaxNumEvents) {
    // Save the original cerr buffer and redirect cerr to a custom stringstream
    std::streambuf *original_cerr_buffer = std::cerr.rdbuf();
    std::ostringstream capture_cerr;
    std::cerr.rdbuf(capture_cerr.rdbuf());

    for (uint32_t i{}; i < 128; ++i) {
        scheduler->addEvent(i, ITimeService::Day::kMonday, 1200, ILightController::State::kOff);
    }

    // Restore the original cerr buffer
    std::cerr.rdbuf(original_cerr_buffer);

    // Compare the captured output with the expected output
    std::string expected_output{};
    EXPECT_THAT(capture_cerr.str(), ::testing::HasSubstr(expected_output));
}

TEST_F(SchedulerTest, ScheduleMoreThan128Events) {
    // Save the original cerr buffer and redirect cerr to a custom stringstream
    std::streambuf *original_cerr_buffer = std::cerr.rdbuf();
    std::ostringstream capture_cerr;
    std::cerr.rdbuf(capture_cerr.rdbuf());

    for (uint32_t i{}; i < 129; ++i) {
        scheduler->addEvent(i, ITimeService::Day::kMonday, 1200, ILightController::State::kOff);
    }

    // Restore the original cerr buffer
    std::cerr.rdbuf(original_cerr_buffer);

    // Compare the captured output with the expected output
    std::string expected_output{"ERROR: Max events number reached"};
    EXPECT_THAT(capture_cerr.str(), ::testing::HasSubstr(expected_output));
}


