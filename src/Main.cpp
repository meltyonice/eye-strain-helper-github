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
#include <Geode/binding/MenuLayer.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/binding/UILayer.hpp>
#include <chrono>
#include <cmath>
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
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "EyeSaverIntegration.hpp"
#include <string>
#include "BreakPopup.hpp"
#include "Settings.hpp"


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

long lastHeartbeat = 0;

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

class $modify(ESHMenuLayer, MenuLayer) {
	bool init() {
		if(!MenuLayer::init()) {
			return false;
		}
		return true;
	}
};

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

		m_fields->timeLoaded = calcNow2();

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		m_fields->eyeStrainHelperUI = CCNode::create();
        m_fields->eyeStrainHelperUI->setTag(10420);
        m_fields->eyeStrainHelperUI->setZOrder(1);

		if(lastHeartbeat == 0 && Settings::shouldIntegrateEyeSaver()) {
			lastHeartbeat = calcNow2();
		} 


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
			if(Settings::timingAlertEnabled()) {
				std::stringstream alertContent;
				long now = calcNow2();
				alertContent<<"Time since epoch: "<<now<<std::endl<<"Time since PlayLayer load: "<<now-m_fields->timeLoaded<<std::endl<<"Time since last break: "<<now-lastBreak<<std::endl<<"Time since mod loaded: "<<now-modLoad<<std::endl<<"Break Duration: "<<Settings::breakDuration();
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
				m_fields->breakPopup = BreakPopup::create(Settings::breakDuration(), 0);
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

		if(calcNow2()-lastHeartbeat >= 10 && Settings::shouldIntegrateEyeSaver()) {
			lastHeartbeat = calcNow2();
			EyeSaverIntegration::sendHeartbeat();
		} 

	}
};



class $modify(ESHPlayerObject, PlayerObject) {
	void playerDestroyed(bool noeffects) {
		PlayerObject::playerDestroyed(noeffects);
		if(calcNow2()-lastBreak >= Settings::minutesBetweenBreaks()*60 || Settings::breakEveryAttempt()) {
			startBreak();
		}
	}

	void update(float deltaTime) {
		if(onBreak) {
			if(Settings::breakDuration() == NULL) {
				Mod::get()->setSettingValue("breakDuration", 30);
			}

			//AppDelegate::get()->pauseSound();

			if(calcNow2()-breakStart >= Settings::breakDuration()) {
				onBreak = false;
				breakJustEnded = true;
			}

		} else {
			PlayerObject::update(deltaTime);
		}
	}
};

class $modify(ESHLevelEditorLayer, LevelEditorLayer) {
	struct Fields {
		CCNode* eyeStrainHelperUI;
		long timeLoaded;
		BreakPopup* breakPopup;
	};

	bool init(GJGameLevel* level, bool noUI) {
		if(!LevelEditorLayer::init(level, noUI)) {
			return false;
		}

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		m_fields->eyeStrainHelperUI = CCNode::create();
        m_fields->eyeStrainHelperUI->setTag(10420);
		m_fields->eyeStrainHelperUI->setID("eye-strain-helper-ui");
        m_fields->eyeStrainHelperUI->setZOrder(1);

		this->addChild(m_fields->eyeStrainHelperUI);

		return true;
	}

	void draw() {
		if(onBreak) {
			if(Settings::breakDuration() == NULL) {
				Mod::get()->setSettingValue("breakDuration", 30);
			}

			//AppDelegate::get()->pauseSound();

			if(calcNow2()-breakStart >= Settings::breakDuration()) {
				onBreak = false;
				breakJustEnded = true;
			}

			if(breakJustStarted) {
				breakJustStarted = false;
				m_fields->breakPopup = BreakPopup::create(Settings::breakDuration(), 1);
			}

			m_fields->breakPopup->update();

			//LevelEditorLayer::pauseAudio();

		} else if((calcNow2()-lastBreak >= Settings::minutesBetweenBreaks()*60 || (Settings::fiveSecondInterval() && calcNow2()-lastBreak >= 5)) && !Settings::shouldIntegrateEyeSaver()) {
			startBreak();
		}
		if(breakJustEnded) {
			//LevelEditorLayer::resumeAudio();
			breakJustEnded = false;
			m_fields->breakPopup->destroy();
		}
		LevelEditorLayer::draw();
	};	
};

class $modify(ESHEditorUI, EditorUI) {
	void onPause(CCObject* sender) {
		if(onBreak) {

		} else {
			EditorUI::onPause(sender);
		}
	}

	void onCreateObject(int id) {
		if(onBreak) {

		} else {
			EditorUI::onCreateObject(id);
		}
	}
};