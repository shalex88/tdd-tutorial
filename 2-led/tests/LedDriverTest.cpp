#include <ostream>
#include "gtest/gtest.h"
/* Add your project include files here */
#include "../source/LedDriver.h"

/* TODO:
 * + All LEDs are off after the driver is initialized
 * + A single LED can be turned on
 * + A single LED can be turned off
 * - Multiple LEDs can be turned on/off
 * - Turn on all LEDs
 * - Turn off all LEDs
 * - Query LED state
 * - Check boundary values
 * - Check out-of-bounds values
 */

TEST(leds, LedsOffOnInit) {
  uint16_t leds_reg_value {0xffff};

  auto led = LedDriver(&leds_reg_value);

  EXPECT_EQ(0, leds_reg_value);
}

TEST(leds, SingleLedOn) {
  uint16_t leds_reg_value {0xffff};

  auto led = LedDriver(&leds_reg_value);
  led.turn_on_num(1);

  EXPECT_EQ(0x1, leds_reg_value);
}

TEST(leds, SingleLedOff) {
  uint16_t leds_reg_value {0xffff};

  auto led = LedDriver(&leds_reg_value);
  led.turn_on_num(1);
  led.turn_off_num(1);

  EXPECT_EQ(0, leds_reg_value);
}
