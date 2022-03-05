#include <utils/os/Time.h>

#include <windows.h>

#include <utils/os/Log.h>

double TimeUtils::GetTime(){
    LARGE_INTEGER time;
    LARGE_INTEGER freq;

    QueryPerformanceCounter(&time);
    QueryPerformanceFrequency(&freq);

    return (double)time.QuadPart / freq.QuadPart;
}

void TimerManager::CreateTimer(int id, int elapseMillis){
    DebugError("TimerManager::CreateTimer [Unimplemented]");
}

void TimerManager::DestroyTimer(int id){
    DebugError("TimerManager::DestroyTimer [Unimplemented]");
}