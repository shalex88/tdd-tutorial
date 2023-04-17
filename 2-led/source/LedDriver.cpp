#include "LedDriver.h"

LedDriver::LedDriver(uint16_t* p_register) : p_leds_reg_(p_register) {
    *p_register = 0;
}

void LedDriver::turn_on_num(const uint8_t led_number) {
    *p_leds_reg_ = *p_leds_reg_ | (1 << (led_number - 1));
}

void LedDriver::turn_off_num(const uint8_t led_number) {
    *p_leds_reg_ = *p_leds_reg_ & ~(1 << (led_number - 1));
}
