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
#include <Geode/binding/UILayer.hpp>
#include <chrono>
#include <ostream>
#include <sstream>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/binding/AppDelegate.hpp>
#include <Geode/modify/PlayerObject.hpp>

#include <string>

using std::string;



long calcNow() {
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

const auto modLoad = calcNow();
auto lastBreak = calcNow();

class $modify(ESHPlayLayer, PlayLayer) {
	struct Fields {
		CCNode* eyeStrainHelperUI;
		CCLabelBMFont* testLabel;
		long timeLoaded;
	};

	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
			return false;
		}

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		m_fields->eyeStrainHelperUI = CCNode::create();
        m_fields->eyeStrainHelperUI->setTag(10420);
        m_fields->eyeStrainHelperUI->setZOrder(1);

		long now = calcNow();
		m_fields->timeLoaded = now;

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
		PlayLayer::pauseGame(unfocused);
		auto timingAlertEnabled = Mod::get()->getSettingValue<bool>("timingAlertEnabled");
		if(timingAlertEnabled) {
			std::stringstream alertContent;
			long now = calcNow();
			alertContent<<"Time since epoch: "<<now<<std::endl<<"Time since PlayLayer load: "<<now-m_fields->timeLoaded<<std::endl<<"Time since last break: "<<now-lastBreak<<std::endl<<"Time since mod loaded: "<<now-modLoad;
			FLAlertLayer::create("Eye Strain Helper", alertContent.str().data(), "OK")->show();
		}
	}

	//void destroyPlayer(PlayerObject* player, GameObject* object) {
	//	auto breakEveryAttempt = Mod::get()->getSettingValue<bool>("breakEveryAttempt");
	//	PlayLayer::destroyPlayer(player, object);
	//	if(breakEveryAttempt) {
	//		FLAlertLayer::create("Eye Strain Helper", "Take a 30 second break! (placeholder)", "OK")->show();
	//	}
	//}
};

class$modify()
