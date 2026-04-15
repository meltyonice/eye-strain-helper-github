#pragma once

using namespace geode::prelude;

class EyeSaverIntegration {
    protected:
        static async::TaskHolder<web::WebResponse> listener;
    public:
        static void sendHeartbeat();
};
