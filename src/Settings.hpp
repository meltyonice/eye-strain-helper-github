#pragma once

#include <cstdint>
class Settings {
    public:
    static bool enabled();
    static bool breakEveryAttempt();
    static bool timingAlertEnabled();
    static bool fiveSecondInterval();
    static bool shouldIntegrateSafeEyes();
    static bool safeEyesBlockInLevels();
    static bool safeEyesOverESHinEditor();
    static bool breakOnPlatformerCP();
    static int64_t breakDuration();
    static int64_t minutesBetweenBreaks();
};