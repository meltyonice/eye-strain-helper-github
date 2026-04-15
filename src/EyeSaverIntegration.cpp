#include "EyeSaverIntegration.hpp"
#include "Geode/loader/Log.hpp"
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

async::TaskHolder<web::WebResponse> EyeSaverIntegration::listener;

void EyeSaverIntegration::sendHeartbeat() {
    web::WebRequest req = geode::utils::web::WebRequest();
    EyeSaverIntegration::listener.spawn(
      req.userAgent("eye-strain-helper").send("GET", "http://localhost:7289/heartbeat"),
       [](geode::utils::web::WebResponse res) {
                log::info("{}", res.string().unwrapOr("FUCKK"));
    });
}