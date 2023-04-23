#include <ostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "../source/LedDriver.h"

/* TODO:
 * + All LEDs are off after the driver is initialized
 * + A single LED can be turned on
 * + A single LED can be turned off
 * + Multiple LEDs can be turned on/off
 * + Turn on all LEDs
 * + Turn off all LEDs
 * + Get single LED state
 * + Check boundary values
 * + Check out-of-bounds values
 */

/* 2 - Test fixture for multiple tests */
class LedDriverTest : public testing::Test {
public:
    LedDriverTest() : led_(LedDriver(&leds_reg_value_)) {};
    ~LedDriverTest() override = default;

    uint16_t leds_reg_value_{0xffff};
    LedDriver led_;
};

TEST_F(LedDriverTest, LedsOffAfterInit) {
    EXPECT_EQ(0, leds_reg_value_);
}

TEST_F(LedDriverTest, SingleLedIsOn) {
    EXPECT_FALSE(led_.isOn(1));

    led_.turnOnNum(1);

    EXPECT_TRUE(led_.isOn(1));
}

TEST_F(LedDriverTest, SingleLedOff) {
    led_.turnOnNum(2);
    led_.turnOffNum(2);

    EXPECT_FALSE(led_.isOn(2));
}

TEST_F(LedDriverTest, MultipleLedsOn) {
    led_.turnOnNum(8);
    led_.turnOnNum(9);

    EXPECT_EQ(0x180, leds_reg_value_);
}

TEST_F(LedDriverTest, TurnOffSomeLed) {
    led_.turnOnNum(8);
    led_.turnOffNum(8);

    EXPECT_FALSE(led_.isOn(8));
}

TEST_F(LedDriverTest, TurnOnAllLeds) {
    led_.turnOnAll();

    EXPECT_EQ(0xffff, leds_reg_value_);
}

TEST_F(LedDriverTest, TurnOffAllLeds) {
    led_.turnOffAll();

    EXPECT_EQ(0, leds_reg_value_);
}

TEST_F(LedDriverTest, LedMemoryIsNotReadable) {
    leds_reg_value_ = 0xffff; /* Simulates changed hw state */
    led_.turnOnNum(8);

    EXPECT_TRUE(led_.isOn(8));
}

TEST_F(LedDriverTest, BoundaryTest) {
    led_.turnOnNum(1);
    led_.turnOnNum(16);

    EXPECT_EQ(0x8001, leds_reg_value_);
}

TEST_F(LedDriverTest, OutOfBoundaryOffTest) {
    led_.turnOnNum(-1);
    led_.turnOnNum(0);
    led_.turnOnNum(17);
    led_.turnOnNum(3141);

    EXPECT_EQ(0, leds_reg_value_);
}

TEST_F(LedDriverTest, OutOfBoundaryOnTest) {
    led_.turnOnAll();

    led_.turnOffNum(-1);
    led_.turnOffNum(0);
    led_.turnOffNum(17);
    led_.turnOffNum(3141);

    EXPECT_EQ(0xffff, leds_reg_value_);
}

TEST_F(LedDriverTest, OutOfBoundaryRuntimeError) {
    // Save the original cerr buffer and redirect cerr to a custom stringstream
    std::streambuf *original_cerr_buffer = std::cerr.rdbuf();
    std::ostringstream capture_cerr;
    std::cerr.rdbuf(capture_cerr.rdbuf());

    // Call the function you want to test
    led_.turnOffNum(0);

    // Restore the original cerr buffer
    std::cerr.rdbuf(original_cerr_buffer);

    // Compare the captured output with the expected output
    std::string expected_output = "ERROR: Invalid LED Number";
    EXPECT_THAT(capture_cerr.str(), ::testing::HasSubstr(expected_output));
}

TEST_F(LedDriverTest, OutOfBoundaryAreOff) {
    EXPECT_FALSE(led_.isOn(0));
    EXPECT_FALSE(led_.isOn(17));
}
