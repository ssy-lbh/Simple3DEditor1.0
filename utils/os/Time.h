#ifndef __UTILS_OS_TIME__
#define __UTILS_OS_TIME__

#include <define.h>

class TimeUtils {
public:
    // time in seconds
    static double GetTime();
};

class TimerManager {
public:
    static void CreateTimer(int id, int elapseMillis);
    static void DestroyTimer(int id);
};

#endif