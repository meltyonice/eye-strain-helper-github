/**
 * Include the Geode headers.
 */
#include "Geode/modify/Modify.hpp"
#include <Geode/Geode.hpp>
#include <Geode/binding/GJBaseGameLayer.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/LabelGameObject.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/UILayer.hpp>

/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;

/**
 * `$modify` lets you extend and modify GD's classes.
 * To hook a function in Geode, simply $modify the class
 * and write a new function definition with the signature of
 * the function you want to hook.
 *
 * Here we use the overloaded `$modify` macro to set our own class name,
 * so that we can use it for button callbacks.
 *
 * Notice the header being included, you *must* include the header for
 * the class you are modifying, or you will get a compile error.
 *
 * Another way you could do this is like this:
 *
 * struct MyMenuLayer : Modify<MyMenuLayer, MenuLayer> {};
 */
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/UILayer.hpp>



class $modify(UILayerMod, UILayer) {
	

	bool init(GJBaseGameLayer* layer) {
		if (!UILayer::init(layer)) {
			return false;
		}

		FLAlertLayer::create("Eye Strain Helper", "Hello from my custom mod!", "OK")->show();

		return true;
	}

};