#ifndef __UTIL_OS_TIME__
#define __UTIL_OS_TIME__

#include <define.h>

namespace simple3deditor {

class Time {
public:
    // ����Ƶ�ϵͳ����ʱ��
    static double GetTime();
    // ��֮֡��ļ��ʱ��(����������OnRender���õ�ʱ����)
    static float GetDeltaTime();
    // ����Ҫ���Ƴ�������ֹ����
    static void Sleep(float time);
};

//TODO δʵ��
class TimerManager {
public:
    static void CreateTimer(int id, int elapseMillis);
    static void DestroyTimer(int id);
};

}

#endif