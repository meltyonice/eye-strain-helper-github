#include "Settings.hpp"
#include <cstdint>

using namespace geode::prelude;

bool Settings::breakEveryAttempt() {
    static bool returnme = (
        listenForSettingChanges<bool>("breakEveryAttempt",[](bool value) { returnme = value; }),
        getMod()->getSettingValue<bool>("breakEveryAttempt")
    );
    return returnme;
}

bool Settings::timingAlertEnabled() {
    static bool returnme = (
        listenForSettingChanges<bool>("timingAlertEnabled",[](bool value) { returnme = value; }),
        getMod()->getSettingValue<bool>("timingAlertEnabled")
    );
    return returnme;
}

int64_t Settings::breakDuration() {
    static bool returnme = (
        listenForSettingChanges<int64_t>("breakDuration",[](int64_t value) { returnme = value; }),
        getMod()->getSettingValue<int64_t>("breakDuration")
    );
    return returnme;
}

int64_t Settings::minutesBetweenBreaks() {
    static bool returnme = (
        listenForSettingChanges<int64_t>("betweenBreaks",[](int64_t value) { returnme = value; }),
        getMod()->getSettingValue<int64_t>("betweenBreaks")
    );
    return returnme;
}