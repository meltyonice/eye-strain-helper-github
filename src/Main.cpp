#include "Geode/cocos/base_nodes/CCNode.h"
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/modify/Modify.hpp"
#include <Geode/Geode.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/GJBaseGameLayer.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/LabelGameObject.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/binding/UILayer.hpp>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <sstream>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/binding/AppDelegate.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/UILayer.hpp>

#include <string>
#include "BreakPopup.hpp"

using std::string;

//long calcNow2() {
//	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//}

//const auto modLoad = calcNow2();
//auto lastBreak = calcNow2();
//long breakStart;
//
//bool onBreak = false;
//bool breakJustStarted = false;

long calcNow2() {
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

long modLoad = calcNow2();
long lastBreak = calcNow2();
long breakStart;

bool onBreak = false;
bool breakJustStarted = false;
bool breakJustEnded = false;

void startBreak() {
	breakStart = calcNow2();
	lastBreak = calcNow2();
	breakJustStarted = true;
	breakJustEnded = false;
	onBreak = true;
}


class $modify(ESHPlayLayer, PlayLayer) {
	struct Fields {
		CCNode* eyeStrainHelperUI;
		long timeLoaded;
		BreakPopup* breakPopup;
	};

	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
			return false;
		}

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		m_fields->eyeStrainHelperUI = CCNode::create();
        m_fields->eyeStrainHelperUI->setTag(10420);
        m_fields->eyeStrainHelperUI->setZOrder(1);


		//std::stringstream labelStr;
		//labelStr<<"Time since load: "<<now-lastBreak;
		//m_fields->testLabel = CCLabelBMFont::create(labelStr.str().data(), "bigFont.fnt");
		//m_fields->testLabel->setPosition({360, winSize.height - 35});
        //m_fields->testLabel->setScale(0.5F);
        //m_fields->testLabel->setOpacity(255 / 2);
		//m_fields->testLabel->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
        //m_fields->testLabel->setZOrder(1000);
        //m_fields->testLabel->setTag(10420); // prevent PlayLayer from interfering
		//m_fields->testLabel->setVisible(Mod::get()->getSettingValue<bool>("enabled"));

		//m_fields->eyeStrainHelperUI->addChild(m_fields->testLabel);
		this->addChild(m_fields->eyeStrainHelperUI);

		return true;
	}

	void pauseGame(bool unfocused) {
		if(!onBreak) {
			PlayLayer::pauseGame(unfocused);
			auto timingAlertEnabled = Mod::get()->getSettingValue<bool>("timingAlertEnabled");
			if(timingAlertEnabled) {
				std::stringstream alertContent;
				long now = calcNow2();
				alertContent<<"Time since epoch: "<<now<<std::endl<<"Time since PlayLayer load: "<<now-m_fields->timeLoaded<<std::endl<<"Time since last break: "<<now-lastBreak<<std::endl<<"Time since mod loaded: "<<now-modLoad;
				FLAlertLayer::create("Eye Strain Helper", alertContent.str().data(), "OK")->show();
			}
		}
	}

	void postUpdate(float deltaTime) {
		PlayLayer::postUpdate(deltaTime);
		if(onBreak) {
			//m_fields->eyeStrainHelperUI
			if(breakJustStarted) {
				breakJustStarted = false;
				auto breakDuration = Mod::get()->getSettingValue<int64_t>("breakDuration");
				m_fields->breakPopup = BreakPopup::create(breakDuration);
			}

			m_fields->breakPopup->update();

			PlayLayer::pauseAudio();
		}
		if(breakJustEnded) {
			PlayLayer::resumeAudio();
			if(PlayLayer::m_isPlatformer) {
				PlayLayer::m_attempts--;
				PlayLayer::resetLevel();
			}
			breakJustEnded = false;
			m_fields->breakPopup->destroy();
		}
	}
};


class $modify(ESHPlayerObject, PlayerObject) {
	void playerDestroyed(bool noeffects) {
		PlayerObject::playerDestroyed(noeffects);
		auto breakEveryAttempt = Mod::get()->getSettingValue<bool>("breakEveryAttempt");
		auto timeBetweenBreaks = Mod::get()->getSettingValue<int64_t>("betweenBreaks");
		if(calcNow2()-lastBreak >= timeBetweenBreaks*60 || breakEveryAttempt) {
			startBreak();
		}
	}

	void update(float deltaTime) {
		if(onBreak) {
			auto breakDuration = Mod::get()->getSettingValue<int64_t>("breakDuration");
			
			if(breakDuration == NULL) {
				Mod::get()->setSettingValue("breakDuration", 30);
				breakDuration = 30;
			}

			//AppDelegate::get()->pauseSound();

			if(calcNow2()-breakStart >= breakDuration) {
				//FLAlertLayer::create("Eye Strain Helper", "Should unpause now!", "OK")->show();
				onBreak = false;
				breakJustEnded = true;
			}

		} else {
			PlayerObject::update(deltaTime);
		}
	}
};