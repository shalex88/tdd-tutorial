#ifndef TDD_TUTORIAL_ITIMESERVICE_H
#define TDD_TUTORIAL_ITIMESERVICE_H

class ITimeService {
public:
    enum class Day {
        kMonday,
        kTuesday,
        kWednsday,
        kThirsday,
        kFriday,
        kSatterday,
        kSunday,
        kEveryday
    };
    struct Time {
        Day day;
        int minute;
    };
    virtual ~ITimeService() = default;
    virtual Time getTime() = 0;
};

#endif //TDD_TUTORIAL_ITIMESERVICE_H
