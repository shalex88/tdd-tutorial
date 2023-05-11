#include "Scheduler.h"

#include <utility>
#include <iostream>
#include <algorithm>

Scheduler::Scheduler(std::shared_ptr<ITimeService> time_service, std::shared_ptr<ILightController> light_controller)
        : time_service_(std::move(time_service)),
          light_controller_(std::move(light_controller)) {
}

std::ostream &operator<<(std::ostream &os, const Event &event) {
    os << "[ " << event.light_id << " " << static_cast<int>(event.light_state) << " "
       << static_cast<int>(event.time.day) << " " << event.time.minute << " ]";
    return os;
}

bool Event::operator==(const Event &rhs) const {
    return light_id == rhs.light_id &&
           (time.day == rhs.time.day || time.day == ITimeService::Day::kEveryday) &&
           time.minute == rhs.time.minute &&
           light_state == rhs.light_state;
}

bool Event::operator!=(const Event &rhs) const {
    return !(rhs == *this);
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

    for (const auto &event: events_) {
        if (((current_date.day == event.time.day) || (event.time.day == ITimeService::Day::kEveryday)) &&
            (current_date.minute == event.time.minute)) {
            if (event.light_state == ILightController::State::kOn) {
                light_controller_->turnOn(event.light_id);
            } else {
                light_controller_->turnOff(event.light_id);
            }
        }
    }
}

std::optional<Event> Scheduler::getLastAddedEvent() const {
    if (events_.empty()) {
        return std::nullopt;
    }
    return events_.back();
}

void Scheduler::removeEvent(const Event &event_to_remove) {
    auto it = std::find(events_.begin(), events_.end(), event_to_remove);
    if (it != events_.end()) {
        events_.erase(it);
    }
}
