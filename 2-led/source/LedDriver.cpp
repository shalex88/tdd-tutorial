#include <iostream>
#include "LedDriver.h"

enum class LedsState {
    kAllOn = ~0,
    kAllOff = ~kAllOn
};

enum class LedsNum {
    kLedFirst = 1,
    kLedLast = 16
};

LedDriver::LedDriver(uint16_t* p_register) : p_leds_reg_(p_register) {
    leds_image_ = static_cast<uint16_t>(LedsState::kAllOff);
    updateHw();
}

uint16_t LedDriver::convertLedNumToBit(const uint8_t led_number) const {
    return (1 << (led_number - 1));
}

void LedDriver::setLedBit(const uint8_t led_number) {
    leds_image_ |= convertLedNumToBit(led_number);
}

void LedDriver::clearLedBit(const uint8_t led_number) {
    leds_image_ &= ~convertLedNumToBit(led_number);
}

void LedDriver::updateHw() const {
    *p_leds_reg_ = leds_image_;
}

void LedDriver::turnOnNum(const uint8_t led_number) {
    if(isValidLed(led_number)) {
        setLedBit(led_number);
        updateHw();
    }
}

void LedDriver::turnOffNum(const uint8_t led_number) {
    if(isValidLed(led_number)) {
        clearLedBit(led_number);
        updateHw();
    }
}

bool LedDriver::isValidLed(const uint8_t led_number) const {
    auto led_num = static_cast<LedsNum>(led_number);
    bool is_valid {led_num >= LedsNum::kLedFirst && led_num <= LedsNum::kLedLast};
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

bool LedDriver::isOn(uint8_t led_number) {
    if(isValidLed(led_number)) {
        return leds_image_ & convertLedNumToBit(led_number);
    }

    return false;
}
