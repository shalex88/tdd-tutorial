#ifndef TDD_TUTORIAL_SCHEDULER_H
#define TDD_TUTORIAL_SCHEDULER_H

#include <cstdint>
#include <memory>
#include <vector>
#include "ITimeService.h"
#include "ILightController.h"

class Scheduler {
public:
    struct Event {
        int light_id;
        ITimeService::Time time;
        ILightController::State light_state;
    };
    Scheduler(std::shared_ptr<ITimeService> time_service, std::shared_ptr<ILightController> light_controller);
    ~Scheduler() = default;
    void addEvent(const uint8_t light_id, const ITimeService::Day day, const int time, const ILightController::State state);
    void triggerEvent();
    Event getLastAddedEvent() const;
private:
    std::shared_ptr<ITimeService> time_service_;
    std::shared_ptr<ILightController> light_controller_;
    std::vector<Event> events_;
};


#endif //TDD_TUTORIAL_SCHEDULER_H
