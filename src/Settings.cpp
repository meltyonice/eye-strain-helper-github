#include "Settings.hpp"
#include <cstdint>

using namespace geode::prelude;

bool Settings::allowSkipping() {
    static bool settingVal = (
        listenForSettingChanges<bool>("allowSkipping",[](bool value) { settingVal = value; }),
        getMod()->getSettingValue<bool>("allowSkipping")
    );
    return settingVal;
}

int64_t Settings::allowedSkips() {
    static int64_t settingVal = (
        listenForSettingChanges<int64_t>("allowedSkips",[](int64_t value) { settingVal = value; }),
        getMod()->getSettingValue<int64_t>("allowedSkips")
    );
    return settingVal;
}

int64_t Settings::breakDuration() {
    static int64_t settingVal = (
        listenForSettingChanges<int64_t>("breakDuration",[](int64_t value) { settingVal = value; }),
        getMod()->getSettingValue<int64_t>("breakDuration")
    );
    return settingVal;
}

int64_t Settings::minutesBetweenBreaks() {
    static int64_t settingVal = (
        listenForSettingChanges<int64_t>("betweenBreaks",[](int64_t value) { settingVal = value; }),
        getMod()->getSettingValue<int64_t>("betweenBreaks")
    );
    return settingVal;
}

bool Settings::breakOnPlatformerCP() {
    static bool settingVal = (
        listenForSettingChanges<bool>("breakOnPlatformerCP",[](bool value) { settingVal = value; }),
        getMod()->getSettingValue<bool>("breakOnPlatformerCP")
    );
    return settingVal;
}

bool Settings::shouldPauseAfterBreak() {
    static bool settingVal = (
        listenForSettingChanges<bool>("shouldPauseAfterBreak",[](bool value) { settingVal = value; }),
        getMod()->getSettingValue<bool>("shouldPauseAfterBreak")
    );
    return settingVal;
}

bool Settings::enabled() {
    static bool settingVal = (
        listenForSettingChanges<bool>("enabled",[](bool value) { settingVal = value; }),
        getMod()->getSettingValue<bool>("enabled")
    );
    return settingVal;
}

// SAFE EYES SETTINGS

bool Settings::safeEyesBlockInLevels() {
    static bool settingVal = (
        listenForSettingChanges<bool>("safeEyesBlockInLevels",[](bool value) { settingVal = value; }),
        getMod()->getSettingValue<bool>("safeEyesBlockInLevels")
    );
    if(getMod()->getSettingValue<bool>("shouldIntegrateSafeEyes")) {
        return settingVal;
    } else {
        return false;
    }
}

bool Settings::safeEyesOverESHinEditor() {
    static bool settingVal = (
        listenForSettingChanges<bool>("safeEyesOverESHinEditor",[](bool value) { settingVal = value; }),
        getMod()->getSettingValue<bool>("safeEyesOverESHinEditor")
    );
    if(getMod()->getSettingValue<bool>("shouldIntegrateSafeEyes")) {
        return settingVal;
    } else {
        return false;
    }
}

//DEBUG SETTINGS

bool Settings::breakEveryAttempt() {
    static bool settingVal = (
        listenForSettingChanges<bool>("breakEveryAttempt",[](bool value) { settingVal = value; }),
        getMod()->getSettingValue<bool>("breakEveryAttempt")
    );
    if(getMod()->getSettingValue<bool>("debugMode")) {
        return settingVal;
    } else {
        return false;
    }
}

bool Settings::instantTowerLoad() {
    static bool settingVal = (
        listenForSettingChanges<bool>("instantTowerLoad",[](bool value) { settingVal = value; }),
        getMod()->getSettingValue<bool>("instantTowerLoad")
    );
    if(getMod()->getSettingValue<bool>("debugMode")) {
        return settingVal;
    } else {
        return false;
    }
}