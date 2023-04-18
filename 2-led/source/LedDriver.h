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
    void turn_on_all();
    void turn_off_all();
private:
    uint16_t* p_leds_reg_ {nullptr};
    uint16_t leds_image;
    void update_hw() const;

    bool isValidLed(const uint8_t led_number) const;
};


#endif //TDD_TUTORIAL_LEDDRIVER_H
