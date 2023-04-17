#ifndef TDD_TUTORIAL_LEDDRIVER_H
#define TDD_TUTORIAL_LEDDRIVER_H

#include <cstdint>

class LedDriver {
public:
    LedDriver() = delete;
    explicit LedDriver(uint16_t* p_register);
    ~LedDriver() = default;
    void turn_on_num(uint8_t led_number);
    void turn_off_num(uint8_t led_number);

private:
    uint16_t* p_leds_reg_ {nullptr};
};


#endif //TDD_TUTORIAL_LEDDRIVER_H
