#pragma once

class EyeStrainHelper {
    public:
        static bool hasShownPingWarningThisLaunch;
        static bool onBreak;
        static void startBreak();
        static bool pingWarningQueued;
};