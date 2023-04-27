#ifndef TDD_TUTORIAL_LEDDRIVER_H
#define TDD_TUTORIAL_LEDDRIVER_H

#include <cstdint>

class LedDriver {
public:
    LedDriver() = delete;
    explicit LedDriver(uint16_t *p_register);
    ~LedDriver() = default;

    void turnOnNum(uint8_t led_number);
    void turnOffNum(uint8_t led_number);
    void turnOnAll();
    void turnOffAll();
    bool isOn(uint8_t led_number);
    uint16_t getRegisterValue() const;

    enum class LedsState {
        kAllOn = ~0,
        kAllOff = ~kAllOn
    };

    enum class LedsNum {
        kLedFirst = 1,
        kLedLast = 16
    };
private:
    uint16_t *p_leds_reg_{nullptr};
    uint16_t leds_image_;
    void updateHw() const;
    bool isValidLed(uint8_t led_number) const;
    void setLedBit(uint8_t led_number);
    void clearLedBit(uint8_t led_number);
    uint16_t convertLedNumToBit(uint8_t led_number) const;
};

#endif //TDD_TUTORIAL_LEDDRIVER_H
