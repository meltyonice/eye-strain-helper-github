#pragma once

using namespace geode::prelude;

class SafeEyesIntegration {
    protected:
        static async::TaskHolder<web::WebResponse> listener;
    public:
        static void sendHeartbeat();
        static void ping();
};
