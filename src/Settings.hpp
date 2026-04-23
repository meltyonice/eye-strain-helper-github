#pragma once

#include <cstdint>
class Settings {
    public:
    static bool enabled();
    static bool breakEveryAttempt();
    static bool allowSkipping();
    static bool safeEyesBlockInLevels();
    static bool safeEyesOverESHinEditor();
    static bool breakOnPlatformerCP();
    static bool shouldPauseAfterBreak();
    static bool instantTowerLoad();
    static int64_t breakDuration();
    static int64_t minutesBetweenBreaks();
    static int64_t allowedSkips();
};