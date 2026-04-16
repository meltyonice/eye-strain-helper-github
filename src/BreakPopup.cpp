#include "GUI/CCControlExtension/CCScale9Sprite.h"
#include "Geode/cocos/base_nodes/CCNode.h"
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/cocos/label_nodes/CCLabelTTF.h"
#include "Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h"
#include "Geode/cocos/sprite_nodes/CCSprite.h"
#include "Geode/loader/Log.hpp"
#include "Geode/ui/Layout.hpp"
#include "Geode/ui/Popup.hpp"
#include "ccTypes.h"
#include <Geode/Geode.hpp>
#include <Geode/binding/BoomListView.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCCountdown.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include <Geode/binding/GJAccountManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GJListLayer.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/binding/UILayer.hpp>
#include <Geode/binding/UploadActionPopup.hpp>
#include <Geode/binding/UploadMessageDelegate.hpp>
#include <cstddef>
#include <cstdlib>
#include <matjson.hpp>
#include <sstream>

#include "Settings.hpp"
#include "BreakPopup.hpp"

using namespace geode::prelude;

long calcNow() {
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

BreakPopup* BreakPopup::create(long breakDuration, int context) {
    auto bp = new BreakPopup;
    if(bp->init(breakDuration, context)) {
        bp->autorelease();
        return bp;
    }
    delete bp;
    return nullptr;
}

long startTime;
CCLabelBMFont* countdown;
CCLayer* popupLayer;
FMOD::ChannelGroup *SoundGroup;
FMOD::Channel *soundChannel;

bool BreakPopup::init(long breakDuration, int context) {
    if(!Popup::init(360.f, 240.f)) { return false; }

    //FLAlertLayer::create("Eye Strain Helper", "Called BreakPopup::init!", "OK")->show();

    popupLayer = CCLayer::create();
    popupLayer->setID("EyeStrainHelperPopupLayer");
    popupLayer->setZOrder(24325);
    startTime = calcNow();

    //auto breakMenuTop = CCSprite::createWithSpriteFrameName("GJ_table_top02_001.png");
    //auto blv = BoomListView::create(NULL, NULL, 0.f, 0.f, 1, BoomListType::Default, 0.f);
    //auto background = GJListLayer::create(blv, "", cocos2d::ccColor4B (0,0,0,180), 356.f, 230.f, 0);
    //background->setPositionX(106.5f);
    //background->setPositionY(40.f);
    

    //auto title = CCLabelBMFont::create("eye strain helper","bigFont.fnt");
    //title->setCString();
    //title->setAnchorPoint(CCPoint (0.5,0.5));
    //title->setPosition(CCPoint (284.5f, 252.f));
    //title->setZOrder(3);
    CCSize boxSize = {360, 90};

    auto background = CCScale9Sprite::create("square01_001.png");
    background->setScale(1.f);
    background->setContentSize({boxSize.width / background->getScale(), boxSize.height / background->getScale()});
    background->setAnchorPoint({0.5f, 0.5f});
    background->setPosition(284.5, 160);
    background->setID("background");

    auto title = CCLabelBMFont::create("eye strain helper","goldFont.fnt");
    title->setAnchorPoint(CCPoint (0.5,0.5));
    title->setPosition(CCPoint (284.5f, 185.f));
    title->setZOrder(3);

    auto bodyText = CCLabelBMFont::create("Look at a point in the far distance for:","chatFont.fnt");
    bodyText->setAnchorPoint(CCPoint (0.5,0.5));
    bodyText->setScale(0.8f);
    bodyText->setPosition(CCPoint (284.5f, 157.f));
    bodyText->setZOrder(3);

    std::stringstream s;
    s<<breakDuration<<"s";
    countdown = CCLabelBMFont::create(s.str().data(),"chatFont.fnt");
    countdown->setAnchorPoint(CCPoint (0.5,0.5));
    countdown->setScale(0.8f);
    countdown->setColor(ccColor3B (255,128,128));
    countdown->setPosition(CCPoint (284.5f, 139.f));
    countdown->setZOrder(3);
    countdown->setID("BreakCountdown");

    popupLayer->setAnchorPoint(CCPoint (0.5f,0.5f));
    popupLayer->setTag(10420);

    //m_mainLayer->addChildAtPosition(rootNode, Anchor::Bottom, {0.f, 30.f});
    popupLayer->addChild(title);
    popupLayer->addChild(bodyText);
    popupLayer->addChild(countdown);
    popupLayer->addChild(background);
    switch (context) {
        case 0:
            PlayLayer::get()->addChild(popupLayer);
            break;
        case 1:
            LevelEditorLayer::get()->addChild(popupLayer);
            break;
        default:
            FLAlertLayer::create("Eye Strain Helper", "Error: tried to create break popup in invalid context", "OK")->show();
            break;
    }
    
    return true;
}

long lastSecond = calcNow();

void BreakPopup::update() {
    //log::info("Drew the popup!");
    if(calcNow()-lastSecond >= 1) {
        lastSecond = calcNow();
        
        std::stringstream s;
        s<<Settings::breakDuration()-(calcNow()-startTime)<<"s";
        countdown->setCString(s.str().data());
        //countdown->setColor(ccColor3B (rand(),rand(),rand()));
        //countdown->updateDisplayedColor(ccColor3B (rand(),rand(),rand()));
        
        //log::info("Changed cstring!");
    }
}

void BreakPopup::destroy() {
    popupLayer->removeMeAndCleanup();
    FMODAudioEngine::sharedEngine()->playEffect("on_stop_break.ogg"_spr);
    //PlayLayer::get()-
}