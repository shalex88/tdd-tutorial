#include <ostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
// Add your project include files here
#include "../source/LedDriver.h"

// TESTS LIST:
// + All LEDs are off after the driver is initialized
// + A single LED can be turned on
// + A single LED can be turned off
// + Multiple LEDs can be turned on/off
// + Turn on all LEDs
// + Turn off all LEDs
// + Get single LED state
// + Check boundary values
// + Check out-of-bounds values

// Test fixture for multiple tests
class LedDriverTest : public testing::Test {
public:
    LedDriverTest() : led(LedDriver(&leds_reg_value)) {};
    ~LedDriverTest() override = default;

    uint16_t leds_reg_value{static_cast<uint16_t>(LedDriver::LedsState::kAllOn)};
    LedDriver led;
};

TEST_F(LedDriverTest, LedsOffAfterInit) {
    EXPECT_EQ(led.getRegisterValue(), leds_reg_value);
}

TEST_F(LedDriverTest, SingleLedIsOn) {
    EXPECT_FALSE(led.isOn(1));

    led.turnOnNum(1);

    EXPECT_TRUE(led.isOn(1));
}

TEST_F(LedDriverTest, SingleLedOff) {
    led.turnOnNum(2);
    led.turnOffNum(2);

    EXPECT_FALSE(led.isOn(2));
}

TEST_F(LedDriverTest, MultipleLedsOn) {
    led.turnOnNum(8);
    led.turnOnNum(9);

    EXPECT_TRUE(led.isOn(8));
    EXPECT_TRUE(led.isOn(9));
}

TEST_F(LedDriverTest, TurnOffSomeLed) {
    led.turnOnNum(8);
    led.turnOffNum(8);

    EXPECT_FALSE(led.isOn(8));
}

TEST_F(LedDriverTest, TurnOnAllLeds) {
    led.turnOnAll();

    EXPECT_EQ(static_cast<uint16_t>(LedDriver::LedsState::kAllOn), led.getRegisterValue());
}

TEST_F(LedDriverTest, TurnOffAllLeds) {
    led.turnOffAll();

    EXPECT_EQ(static_cast<uint16_t>(LedDriver::LedsState::kAllOff), led.getRegisterValue());
}

TEST_F(LedDriverTest, LedMemoryIsNotReadable) {
    leds_reg_value = static_cast<uint16_t>(LedDriver::LedsState::kAllOn); // Simulates changed hw state

    EXPECT_FALSE(led.isOn(8));
}

TEST_F(LedDriverTest, BoundaryTest) {
    led.turnOnNum(1);
    led.turnOnNum(16);

    EXPECT_TRUE(led.isOn(1));
    EXPECT_TRUE(led.isOn(16));
}

TEST_F(LedDriverTest, OutOfBoundaryOffTest) {
    led.turnOnNum(-1);
    led.turnOnNum(0);
    led.turnOnNum(17);
    led.turnOnNum(3141);

    EXPECT_EQ(static_cast<uint16_t>(LedDriver::LedsState::kAllOff), led.getRegisterValue());
}

TEST_F(LedDriverTest, OutOfBoundaryOnTest) {
    led.turnOnAll();

    led.turnOffNum(-1);
    led.turnOffNum(0);
    led.turnOffNum(17);
    led.turnOffNum(3141);

    EXPECT_EQ(static_cast<uint16_t>(LedDriver::LedsState::kAllOn), led.getRegisterValue());
}

TEST_F(LedDriverTest, OutOfBoundaryRuntimeError) {
    // Save the original cerr buffer and redirect cerr to a custom stringstream
    std::streambuf *original_cerr_buffer = std::cerr.rdbuf();
    std::ostringstream capture_cerr;
    std::cerr.rdbuf(capture_cerr.rdbuf());

    // Call the function you want to test
    led.turnOffNum(0);

    // Restore the original cerr buffer
    std::cerr.rdbuf(original_cerr_buffer);

    // Compare the captured output with the expected output
    std::string expected_output = "ERROR: Invalid LED Number";
    EXPECT_THAT(capture_cerr.str(), ::testing::HasSubstr(expected_output));
}

TEST_F(LedDriverTest, OutOfBoundaryAreOff) {
    EXPECT_FALSE(led.isOn(0));
    EXPECT_FALSE(led.isOn(17));
}
