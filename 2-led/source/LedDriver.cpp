#include <iostream>
#include "LedDriver.h"

enum class LedsState {
    kAllOn = ~0,
    kAllOff = ~kAllOn
};

LedDriver::LedDriver(uint16_t* p_register) : p_leds_reg_(p_register) {
    leds_image_ = static_cast<uint16_t>(LedsState::kAllOff);
    updateHw();
}

void LedDriver::updateHw() const {
    *p_leds_reg_ = leds_image_;
}

void LedDriver::turnOnNum(const uint8_t led_number) {
    if(isValidLed(led_number)) {
        leds_image_ |= (1 << (led_number - 1));
        updateHw();
    }
}

void LedDriver::turnOffNum(const uint8_t led_number) {
    if(isValidLed(led_number)) {
        leds_image_ &= ~(1 << (led_number - 1));
        updateHw();
    }
}

bool LedDriver::isValidLed(const uint8_t led_number) const {
    bool is_valid {led_number >= 1 && led_number <= 16};
    if (!is_valid) {
        std::cerr << "ERROR: Invalid LED Number " << static_cast<int>(led_number) << std::endl;
    }
    return is_valid;
}

void LedDriver::turnOnAll() {
    leds_image_ = static_cast<uint16_t>(LedsState::kAllOn);
    updateHw();
}

void LedDriver::turnOffAll() {
    leds_image_ = static_cast<uint16_t>(LedsState::kAllOff);
    updateHw();
}
