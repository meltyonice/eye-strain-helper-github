#include "Main.hpp"
#include "Geode/utils/cocos.hpp"
#include "SafeEyesIntegration.hpp"
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <fmt/format.h>
#include <stdlib.h>
#include "BreakPopup.hpp"
#include "Settings.hpp"

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/CheckpointGameObject.hpp>

long EyeStrainHelper::calcNow() {
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

long EyeStrainHelper::lastHeartbeat = 0;

long modLoad = EyeStrainHelper::calcNow();
long lastBreak = EyeStrainHelper::calcNow();
long breakStart;

bool EyeStrainHelper::onBreak = false;
bool EyeStrainHelper::pingWarningQueued = false;
bool EyeStrainHelper::hasShownPingWarningThisLaunch = false;
bool EyeStrainHelper::breakJustEnded = false;
bool breakJustStarted = false;
bool hasInstaLoadedTower = false;
bool inputLockCapturedState = false;

long sinceUnsafeOperation = 0;

bool isPopupSafe() {
	return EyeStrainHelper::calcNow()-sinceUnsafeOperation >= 1;
}

int EyeStrainHelper::concurrentSkips = 0;

BreakPopup* breakPopup;

void EyeStrainHelper::startBreak() {
	breakStart = EyeStrainHelper::calcNow();
	lastBreak = EyeStrainHelper::calcNow();
	breakJustStarted = true;
	EyeStrainHelper::breakJustEnded = false;
	EyeStrainHelper::onBreak = true;
}

class $modify(ESHCheckpointGameObject, CheckpointGameObject) {
	void triggerObject(GJBaseGameLayer* layer, int uniqueID, gd::vector<int> const* remapKeys) {
		CheckpointGameObject::triggerObject(layer, uniqueID, remapKeys);
		if(Settings::breakOnPlatformerCP() && Settings::enabled() && EyeStrainHelper::calcNow()-lastBreak >= Settings::minutesBetweenBreaks()*60) {
			EyeStrainHelper::startBreak();
		}
	}
};

#ifdef GEODE_IS_WINDOWS
class $modify(ESHMenuLayer, MenuLayer) {
    void onPlay(CCObject* sender) {
        if(Settings::enabled()) {
            if(Settings::instantTowerLoad() && !hasInstaLoadedTower) {
                auto LevelManager = GameLevelManager::sharedState();
                auto tower = LevelManager->getMainLevel(5001, false);
                auto pl = PlayLayer::scene(tower, false, false);
                hasInstaLoadedTower = true;
				auto ccd = CCDirector::get();
                ccd->replaceScene(pl);
            } else {
                MenuLayer::onPlay(sender);
            }
        }
    }
	
};
#endif

class $modify(ESHPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        if(Settings::enabled()) {
            if(EyeStrainHelper::lastHeartbeat == 0 && Settings::safeEyesBlockInLevels()) {
                SafeEyesIntegration::sendHeartbeat();
		    } 
        }
        return true;
    }

    void pauseGame(bool unfocused) {
        if(!EyeStrainHelper::onBreak) {
            PlayLayer::pauseGame(unfocused);
        }
    }
};

class $modify(ESHEditorUI, EditorUI) {
	bool init(LevelEditorLayer* editorLayer) {
		if (!EditorUI::init(editorLayer)) return false;
		sinceUnsafeOperation = EyeStrainHelper::calcNow();
		return true;
	}

    void draw() {
        //log::debug("Called!");
		if((EyeStrainHelper::calcNow()-lastBreak >= Settings::minutesBetweenBreaks()*60) && !Settings::safeEyesOverESHinEditor() && isPopupSafe()) {
			EyeStrainHelper::startBreak();
		}

		if(EyeStrainHelper::calcNow()-EyeStrainHelper::lastHeartbeat >= 10 && !Settings::safeEyesOverESHinEditor() ) {
			SafeEyesIntegration::sendHeartbeat();
		} 
        if(EyeStrainHelper::onBreak) {
            if(breakJustStarted) {
				breakJustStarted = false;
				breakPopup = BreakPopup::create(Settings::breakDuration());
		    }

			if(isPopupSafe()) {
				breakPopup->update();
			}

            if(EyeStrainHelper::calcNow()-breakStart >= Settings::breakDuration()) {
		    	EyeStrainHelper::onBreak = false;
		    	EyeStrainHelper::breakJustEnded = true;
		    }

            if(EyeStrainHelper::breakJustEnded) {
			    EyeStrainHelper::breakJustEnded = false;
			    breakPopup->destroy(false);
		    }
        }
        EditorUI::draw();
    }
};

class $modify(ESHBaseGameLayer, GJBaseGameLayer) {
    void update(float dt) {
        if(Settings::enabled()) {
        if(!EyeStrainHelper::onBreak) {
            if(EyeStrainHelper::calcNow()-EyeStrainHelper::lastHeartbeat >= 10 && Settings::safeEyesBlockInLevels()) {
			    SafeEyesIntegration::sendHeartbeat();
		    }
            GJBaseGameLayer::update(dt);
        } else {
        if(!m_isEditor || m_isTestMode) {
            if(EyeStrainHelper::calcNow()-EyeStrainHelper::lastHeartbeat >= 10 && Settings::safeEyesBlockInLevels()) {
			    SafeEyesIntegration::sendHeartbeat();
		    }
			if(breakJustStarted) {
				breakJustStarted = false;
				breakPopup = BreakPopup::create(Settings::breakDuration());
			}
			breakPopup->update();

			PlayLayer::get()->pauseAudio();

			if(EyeStrainHelper::calcNow()-breakStart >= Settings::breakDuration()) {
				EyeStrainHelper::onBreak = false;
				EyeStrainHelper::breakJustEnded = true;
			}

            if(EyeStrainHelper::breakJustEnded) {
			    PlayLayer::get()->resumeAudio();
				if(Settings::shouldPauseAfterBreak()) {
					PlayLayer::get()->pauseGame(false);
				}
			    EyeStrainHelper::breakJustEnded = false;
			    breakPopup->destroy(false);
		    }
        }
        }
    } else {
		GJBaseGameLayer::update(dt);
	}
    }
};

class $modify(ESHPlayerObject, PlayerObject) {
    void playerDestroyed(bool noeffects) {
		PlayerObject::playerDestroyed(noeffects);
		if(Settings::enabled()) {
			if(EyeStrainHelper::calcNow()-lastBreak >= Settings::minutesBetweenBreaks()*60 || Settings::breakEveryAttempt()) {
				EyeStrainHelper::startBreak();
			}
		}
	}
};