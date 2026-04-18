#pragma once

#include <cstdint>
class Settings {
    public:
    static bool enabled();
    static bool breakEveryAttempt();
    static bool fiveSecondInterval();
    static bool allowSkipping();
    static bool shouldIntegrateSafeEyes();
    static bool safeEyesBlockInLevels();
    static bool safeEyesOverESHinEditor();
    static bool breakOnPlatformerCP();
    static bool instantTowerLoad();
    static int64_t breakDuration();
    static int64_t minutesBetweenBreaks();
    static int64_t allowedSkips();
};