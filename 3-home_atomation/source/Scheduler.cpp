#include "Scheduler.h"

#include <utility>
#include <iostream>

Scheduler::Scheduler(std::shared_ptr<ITimeService> time_service, std::shared_ptr<ILightController> light_controller)
        : time_service_(std::move(time_service)),
          light_controller_(std::move(light_controller)) {
}

void Scheduler::addEvent(const uint8_t light_id, const ITimeService::Day day, const int time,
                         const ILightController::State state) {
    Event event{light_id,
                {day, time},
                state};
    events_.push_back(event);
}

void Scheduler::triggerEvent() {
    auto current_date = time_service_->getTime();

    for (auto it = events_.begin(); it != events_.end();) {
        if (((current_date.day == it->time.day) || (it->time.day == ITimeService::Day::kEveryday)) &&
            (current_date.minute == it->time.minute)) {
            if (it->light_state == ILightController::State::kOn) {
                light_controller_->turnOn(it->light_id);
            } else {
                light_controller_->turnOff(it->light_id);
            }
        }
        ++it;
    }
}

Scheduler::Event Scheduler::getLastAddedEvent() const {
    std::cout << events_.back() << std::endl;
    return events_.back();
}

std::ostream &operator<<(std::ostream &os, const Scheduler::Event &event) {
    os << "[ " << event.light_id << " " << static_cast<int>(event.light_state) << " "
       << static_cast<int>(event.time.day) << " " << event.time.minute << " ]";
    return os;
}
