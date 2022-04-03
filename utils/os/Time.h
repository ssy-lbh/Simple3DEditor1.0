#ifndef __UTILS_OS_TIME__
#define __UTILS_OS_TIME__

#include <define.h>

class Time {
public:
    // 以秒计的系统绝对时间
    static double GetTime();
    // 两帧之间的间隔时间(可视作两次OnRender调用的时间间隔)
    static float GetDeltaTime();
};

class TimerManager {
public:
    static void CreateTimer(int id, int elapseMillis);
    static void DestroyTimer(int id);
};

#endif