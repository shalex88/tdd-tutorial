#ifndef TDD_TUTORIAL_ILIGHTCONTROLLER_H
#define TDD_TUTORIAL_ILIGHTCONTROLLER_H

#include <cinttypes>

class ILightController {
public:
    enum class State : bool {
        kOn,
        kOff
    };
    virtual ~ILightController() = default;
    virtual void turnOn(uint8_t light_id) = 0;
    virtual void turnOff(uint8_t light_id) = 0;
    virtual bool getStatus(uint8_t light_id) = 0;
};

#endif //TDD_TUTORIAL_ILIGHTCONTROLLER_H
