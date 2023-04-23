#ifndef TDD_TUTORIAL_LEDDRIVER_H
#define TDD_TUTORIAL_LEDDRIVER_H

#include <cstdint>

class LedDriver {
public:
    LedDriver() = delete;
    explicit LedDriver(uint16_t* p_register);
    ~LedDriver() = default;
    void turnOnNum(uint8_t led_number);
    void turnOffNum(uint8_t led_number);
    void turnOnAll();
    void turnOffAll();
private:
    uint16_t* p_leds_reg_ {nullptr};
    uint16_t leds_image_;
    void updateHw() const;
    bool isValidLed(uint8_t led_number) const;
};


#endif //TDD_TUTORIAL_LEDDRIVER_H
