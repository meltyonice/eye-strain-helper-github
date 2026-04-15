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

bool Settings::fiveSecondInterval() {
    static bool returnme = (
        listenForSettingChanges<bool>("fiveSecondInterval",[](bool value) { returnme = value; }),
        getMod()->getSettingValue<bool>("fiveSecondInterval")
    );
    return returnme;
}

bool Settings::shouldIntegrateEyeSaver() {
    static bool returnme = (
        listenForSettingChanges<bool>("shouldIntegrateEyeSaver",[](bool value) { returnme = value; }),
        getMod()->getSettingValue<bool>("shouldIntegrateEyeSaver")
    );
    return returnme;
}

int64_t Settings::breakDuration() {
    static int64_t returnme = (
        listenForSettingChanges<int64_t>("breakDuration",[](int64_t value) { returnme = value; }),
        getMod()->getSettingValue<int64_t>("breakDuration")
    );
    return returnme;
}

int64_t Settings::minutesBetweenBreaks() {
    static int64_t returnme = (
        listenForSettingChanges<int64_t>("betweenBreaks",[](int64_t value) { returnme = value; }),
        getMod()->getSettingValue<int64_t>("betweenBreaks")
    );
    return returnme;
}