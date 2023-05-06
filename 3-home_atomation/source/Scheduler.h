#ifndef TDD_TUTORIAL_SCHEDULER_H
#define TDD_TUTORIAL_SCHEDULER_H

#include <cstdint>
#include "ITimeService.h"
#include "ILightController.h"

class Scheduler {
public:
    struct Event {
        int light_id;
        ITimeService::Time time;
        ILightController::State light_state;
    };
    Scheduler(ITimeService *time_service, ILightController *light_controller);
    ~Scheduler() = default;
    void addEvent(uint8_t light_id, ITimeService::Day day, int time, ILightController::State state);
    void triggerEvent();
    Event getNextEvent() const;
private:
    ITimeService *time_service_;
    ILightController *light_controller_;
    Event event_ {};
};


#endif //TDD_TUTORIAL_SCHEDULER_H
