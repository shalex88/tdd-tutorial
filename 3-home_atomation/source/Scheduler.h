#ifndef TDD_TUTORIAL_SCHEDULER_H
#define TDD_TUTORIAL_SCHEDULER_H

#include <cstdint>
#include <memory>
#include <vector>
#include <ostream>
#include <optional>
#include "ITimeService.h"
#include "ILightController.h"

struct Event {
    int light_id;
    ITimeService::Time time;
    ILightController::State light_state;
};

std::ostream &operator<<(std::ostream &os, const Event &event);

class Scheduler {
public:

    Scheduler(std::shared_ptr<ITimeService> time_service, std::shared_ptr<ILightController> light_controller);
    ~Scheduler() = default;
    void addEvent(const uint8_t light_id, const ITimeService::Day day, const int time, const ILightController::State state);
    void removeEvent(const Event &event_to_remove);
    void triggerEvent();
    std::optional<Event> getLastAddedEvent() const;

private:
    std::shared_ptr<ITimeService> time_service_;
    std::shared_ptr<ILightController> light_controller_;
    std::vector<Event> events_{};
};


#endif //TDD_TUTORIAL_SCHEDULER_H
