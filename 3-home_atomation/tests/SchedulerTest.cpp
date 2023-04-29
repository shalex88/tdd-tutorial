#include <ostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
// Add your project include files here

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

TEST(SchedulerTest, TestFrameworkIsWorking) {
    EXPECT_TRUE(true);
}
