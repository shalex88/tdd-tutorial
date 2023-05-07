#include "Scheduler.h"

Scheduler::Scheduler(std::shared_ptr<ITimeService> time_service, std::shared_ptr<ILightController> light_controller)
        : time_service_(time_service),
          light_controller_(light_controller) {

}

void Scheduler::addEvent(const uint8_t light_id, const ITimeService::Day day, const int time, const ILightController::State state) {
    Event event{light_id,
                {day, time},
                state};
    events_.push_back(event);
}

void Scheduler::triggerEvent() {
    auto time = time_service_->getTime();

    for (auto it = events_.begin(); it != events_.end();) {
        if ((time.day == it->time.day) && (time.minute == it->time.minute)) {
            if (it->light_state == ILightController::State::kOn) {
                light_controller_->turnOn(it->light_id);
            } else {
                light_controller_->turnOff(it->light_id);
            }
            it = events_.erase(it);
        } else {
            ++it;
        }
    }
}

Scheduler::Event Scheduler::getLastAddedEvent() const {
    return events_.back();
}
