#pragma once

#include <cstdint>
class Settings {
    public:
    static bool breakEveryAttempt();
    static bool timingAlertEnabled();
    static bool fiveSecondInterval();
    static int64_t breakDuration();
    static int64_t minutesBetweenBreaks();
};