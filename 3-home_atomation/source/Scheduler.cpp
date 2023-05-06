#include "Scheduler.h"

Scheduler::Scheduler(std::shared_ptr<ITimeService> time_service, std::shared_ptr<ILightController> light_controller)
        : time_service_(time_service),
          light_controller_(light_controller) {

}

void Scheduler::addEvent(const uint8_t light_id, const ITimeService::Day day, const int time, const ILightController::State state) {
    event_.light_id = light_id;
    event_.time.day = day;
    event_.time.minute = time;
    event_.light_state = state;
}

void Scheduler::triggerEvent() const {
    auto time = time_service_->getTime();
    if ((time.day == event_.time.day) && (time.minute == event_.time.minute)) {
        if (event_.light_state == ILightController::State::kOn) {
            light_controller_->turnOn(event_.light_id);
        } else {
            light_controller_->turnOff(event_.light_id);
        }
    }
}

Scheduler::Event Scheduler::getNextEvent() const {
    return event_;
}
