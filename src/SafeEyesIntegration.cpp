#include "SafeEyesIntegration.hpp"
#include "Geode/loader/Log.hpp"
#include <Geode/utils/web.hpp>
#include "Main.hpp"

using namespace geode::prelude;

async::TaskHolder<web::WebResponse> SafeEyesIntegration::listener;

void SafeEyesIntegration::sendHeartbeat() {
    EyeStrainHelper::lastHeartbeat = EyeStrainHelper::calcNow();
    web::WebRequest req = geode::utils::web::WebRequest();
    SafeEyesIntegration::listener.spawn(
      req.header("User-Agent","eye-strain-helper").send("HEARTBEAT", "http://localhost:7289/"),
       [](geode::utils::web::WebResponse res) {
                log::info("{}", res.string().unwrapOr("Fiddlesticks!"));
    });
}

void SafeEyesIntegration::ping() {
    web::WebRequest req = geode::utils::web::WebRequest();
    SafeEyesIntegration::listener.spawn(
      req.header("User-Agent","eye-strain-helper").send("PING", "http://localhost:7289/"),
       [](geode::utils::web::WebResponse res) {
                if(res.code() != 200) {
                    EyeStrainHelper::hasShownPingWarningThisLaunch = true;
                    EyeStrainHelper::pingWarningQueued = true;
                }
    });
}