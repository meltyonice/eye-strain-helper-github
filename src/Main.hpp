#pragma once

class EyeStrainHelper {
    public:
        static bool hasShownPingWarningThisLaunch;
        static bool onBreak;
        static void startBreak();
        static bool pingWarningQueued;
        static long calcNow();
        static long lastHeartbeat;
        static bool breakJustEnded;
        static int concurrentSkips;
};