#include <iostream>
//#include <error.h>
#include "LedDriver.h"

enum class LEDS_STATE {
    ALL_ON = ~0,
    ALL_OFF = ~ALL_ON
};

LedDriver::LedDriver(uint16_t* p_register) : p_leds_reg_(p_register) {
    leds_image = static_cast<uint16_t>(LEDS_STATE::ALL_OFF);
    update_hw();
}

void LedDriver::update_hw() const {
    *p_leds_reg_ = leds_image;
}

void LedDriver::turn_on_num(const uint8_t led_number) {
    if(isValidLed(led_number)) {
        leds_image |= (1 << (led_number - 1));
        update_hw();
    }
}

void LedDriver::turn_off_num(const uint8_t led_number) {
    if(isValidLed(led_number)) {
        leds_image &= ~(1 << (led_number - 1));
        update_hw();
    }
}

bool LedDriver::isValidLed(const uint8_t led_number) const {
    bool is_valid {led_number >= 1 && led_number <= 16};
    if (!is_valid) {
        std::cerr << "ERROR: Invalid LED Number " << static_cast<int>(led_number) << std::endl;
    }
    return is_valid;
}

void LedDriver::turn_on_all() {
    leds_image = static_cast<uint16_t>(LEDS_STATE::ALL_ON);
    update_hw();
}

void LedDriver::turn_off_all() {
    leds_image = static_cast<uint16_t>(LEDS_STATE::ALL_OFF);
    update_hw();
}
