#pragma once

#include "Geode/cocos/cocoa/CCGeometry.h"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h"
#include "Geode/cocos/menu_nodes/CCMenu.h"
#include "Geode/loader/Formatter.hpp"
#include "Geode/loader/Log.hpp"
#include "Geode/ui/Layout.hpp"
#include "Geode/ui/NineSlice.hpp"
#include "Main.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <fmt/format.h>
#include "Settings.hpp"
using namespace geode::prelude;

class BreakPopup : public geode::Popup {
protected:

    CCLabelBMFont* timerLabel;
    long startTime;
    long durationHelper;

    std::string remainingTimeFormatted() {
        auto secondsRemaining = durationHelper-(EyeStrainHelper::calcNow()-startTime);
        auto minutes = secondsRemaining/60;
        auto secondsOnMinute = secondsRemaining-(minutes*60);
        std::string formattedSecondsOnMinute;
        if(secondsOnMinute < 10) {
            formattedSecondsOnMinute = fmt::format("0{}", secondsOnMinute);
        } else {
            formattedSecondsOnMinute = fmt::format("{}", secondsOnMinute);
        }
        return fmt::format("{}:{}", minutes, formattedSecondsOnMinute);
    }

    bool canSkipThisBreak() {
        return (EyeStrainHelper::concurrentSkips < Settings::allowedSkips() || Settings::allowedSkips() == 0);
    }

    bool init(int duration, std::string breakMessage) {
        if (!Popup::init(240.f, 150.f))
            return false;

        durationHelper = duration;

        this->setTitle("");
        this->setID("ESHBreakPopup"_spr);
        auto ccm = this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0);                              
        ccm->getChildByType<CCMenuItemSpriteExtra>(0)->setID("closebtn-destroyme"_spr); 
        m_bgSprite->setID("bgspr-destroyme"_spr);
        ccm->removeChildByID("closebtn-destroyme"_spr);
        m_mainLayer->removeChildByID("bgspr-destroyme"_spr);
        

        auto label = CCLabelBMFont::create(breakMessage.c_str(), "bigFont.fnt");
        label->setScale(0.6f);
        m_mainLayer->addChildAtPosition(label, Anchor::Top);

        startTime = EyeStrainHelper::calcNow();
        timerLabel = CCLabelBMFont::create(remainingTimeFormatted().c_str(), "bigFont.fnt");
        timerLabel->setScale(0.8f);
        m_mainLayer->addChildAtPosition(timerLabel, Anchor::Center);

        if(Settings::allowSkipping()) {
            if(canSkipThisBreak()) {
                auto skipSpr = ButtonSprite::create("Skip");
                skipSpr->updateBGImage("geode.loader/GE_button_05.png");
                auto skipButton = CCMenuItemSpriteExtra::create(
                    skipSpr,
                    this,
                    menu_selector(BreakPopup::onSkipBreak)
                );
                skipButton->setAnchorPoint(CCPoint(0.5,1.0));
                ccm->addChildAtPosition(skipButton, Anchor::Bottom);
            }
            auto skipsLabel = CCLabelBMFont::create(fmt::format("Concurrent Skips: {}/{}", EyeStrainHelper::concurrentSkips, Settings::allowedSkips()).c_str(), "chatFont.fnt");
            skipsLabel->setAnchorPoint(CCPoint(0.5,0.0));
            m_mainLayer->addChildAtPosition(skipsLabel, Anchor::Bottom);
        }

        this->show();
        return true;
    }

    void keyBackClicked() {}

    void onSkipBreak(CCObject* sender) {
        EyeStrainHelper::onBreak = false;
        this->destroy(true);
    }

public:
    static BreakPopup* create(int duration, std::string breakMessage = "Look at a point far in the distance") {
        auto ret = new BreakPopup();
        if (ret->init(duration, breakMessage)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

    void update() {
        timerLabel->setString(remainingTimeFormatted().c_str());
    }

    void destroy(bool justSkippedBreak) {
        if(justSkippedBreak) {
            EyeStrainHelper::concurrentSkips++;
            justSkippedBreak = false;
        } else {
            FMODAudioEngine::sharedEngine()->playEffect("on_stop_break.ogg"_spr);
            EyeStrainHelper::concurrentSkips = 0;
        }
        this->onClose(this);
    }
};