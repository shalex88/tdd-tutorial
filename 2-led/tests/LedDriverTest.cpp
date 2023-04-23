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
 * - Query LED state
 * + Check boundary values
 * - Check out-of-bounds values
 */

/* 2 - Test fixture for multiple tests */
class LedDriverTest : public testing::Test {
public:
    uint16_t leds_reg_value {0xffff};
    LedDriver led;
    LedDriverTest() : led(LedDriver(&leds_reg_value)) {};
    ~LedDriverTest() override = default;
};

TEST_F(LedDriverTest, LedsOffAfterInit) {
  EXPECT_EQ(0, leds_reg_value);
}

TEST_F(LedDriverTest, SingleLedOn) {
    led.turnOnNum(1);

  EXPECT_EQ(0x1, leds_reg_value);
}

TEST_F(LedDriverTest, SingleLedOff) {
    led.turnOnNum(2);
    led.turnOffNum(2);

  EXPECT_EQ(0, leds_reg_value);
}

TEST_F(LedDriverTest, MultipleLedsOn) {
    led.turnOnNum(8);
    led.turnOnNum(9);

  EXPECT_EQ(0x180, leds_reg_value);
}

TEST_F(LedDriverTest, TurnOffSomeLed) {
    led.turnOnNum(8);
    led.turnOnNum(9);
    led.turnOffNum(8);

  EXPECT_EQ(0x100, leds_reg_value);
}

TEST_F(LedDriverTest, TurnOnAllLeds) {
    led.turnOnAll();

  EXPECT_EQ(0xffff, leds_reg_value);
}

TEST_F(LedDriverTest, TurnOffAllLeds) {
    led.turnOffAll();

  EXPECT_EQ(0, leds_reg_value);
}

TEST_F(LedDriverTest, LedMemoryIsNotReadable) {
  leds_reg_value = 0xffff; /* Simulates changed hw state */
    led.turnOnNum(8);

  EXPECT_EQ(0x80, leds_reg_value);
}

TEST_F(LedDriverTest, BoundaryTest) {
    led.turnOnNum(1);
    led.turnOnNum(16);

  EXPECT_EQ(0x8001, leds_reg_value);
}

TEST_F(LedDriverTest, OutOfBoundaryOnTest) {
    led.turnOnNum(-1);
    led.turnOnNum(0);
    led.turnOnNum(17);
    led.turnOnNum(3141);

  EXPECT_EQ(0, leds_reg_value);
}

TEST_F(LedDriverTest, OutOfBoundaryOffTest) {
    led.turnOnAll();

    led.turnOffNum(-1);
    led.turnOffNum(0);
    led.turnOffNum(17);
    led.turnOffNum(3141);

  EXPECT_EQ(0xffff, leds_reg_value);
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
