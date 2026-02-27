#pragma once
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/ui/Popup.hpp"
#include <Geode/Geode.hpp>
#include <Geode/binding/GJGameLevel.hpp>

class BreakPopup : public geode::Popup {
    protected:
        bool init(long breakDuration);
        
    public:
        static BreakPopup* create(long breakDuration);
        void update();
        void destroy();
};