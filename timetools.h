#ifndef __TIMETOOLS__
#define __TIMETOOLS__

class TimeUtils;
class TimerManager;

class TimeUtils {
public:
    static float GetTime();
};

class TimerManager {
public:
    static void CreateTimer(int id, int elapseMillis);
    static void DestroyTimer(int id);
};

#endif