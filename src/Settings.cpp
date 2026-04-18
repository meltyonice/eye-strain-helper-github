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

bool Settings::fiveSecondInterval() {
    static bool returnme = (
        listenForSettingChanges<bool>("fiveSecondInterval",[](bool value) { returnme = value; }),
        getMod()->getSettingValue<bool>("fiveSecondInterval")
    );
    return returnme;
}


bool Settings::instantTowerLoad() {
    static bool returnme = (
        listenForSettingChanges<bool>("instantTowerLoad",[](bool value) { returnme = value; }),
        getMod()->getSettingValue<bool>("instantTowerLoad")
    );
    return returnme;
}

bool Settings::allowSkipping() {
    static bool returnme = (
        listenForSettingChanges<bool>("allowSkipping",[](bool value) { returnme = value; }),
        getMod()->getSettingValue<bool>("allowSkipping")
    );
    return returnme;
}

int64_t Settings::allowedSkips() {
    static int64_t returnme = (
        listenForSettingChanges<int64_t>("allowedSkips",[](int64_t value) { returnme = value; }),
        getMod()->getSettingValue<int64_t>("allowedSkips")
    );
    return returnme;
}


bool Settings::shouldIntegrateSafeEyes() {
    static bool returnme = (
        listenForSettingChanges<bool>("shouldIntegrateSafeEyes",[](bool value) { returnme = value; }),
        getMod()->getSettingValue<bool>("shouldIntegrateSafeEyes")
    );
    return returnme;
}

bool Settings::safeEyesBlockInLevels() {
    static bool returnme = (
        listenForSettingChanges<bool>("safeEyesBlockInLevels",[](bool value) { returnme = value; }),
        getMod()->getSettingValue<bool>("safeEyesBlockInLevels")
    );
    return returnme;
}

bool Settings::safeEyesOverESHinEditor() {
    static bool returnme = (
        listenForSettingChanges<bool>("safeEyesOverESHinEditor",[](bool value) { returnme = value; }),
        getMod()->getSettingValue<bool>("safeEyesOverESHinEditor")
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

bool Settings::breakOnPlatformerCP() {
    static bool returnme = (
        listenForSettingChanges<bool>("breakOnPlatformerCP",[](bool value) { returnme = value; }),
        getMod()->getSettingValue<bool>("breakOnPlatformerCP")
    );
    return returnme;
}

bool Settings::enabled() {
    static bool returnme = (
        listenForSettingChanges<bool>("enabled",[](bool value) { returnme = value; }),
        getMod()->getSettingValue<bool>("enabled")
    );
    return returnme;
}