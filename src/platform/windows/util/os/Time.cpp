#include <util/os/Time.h>

#include <windows.h>

#include <main.h>
#include <util/os/Log.h>
#include <util/math3d/Math.h>

double Time::GetTime(){
    LARGE_INTEGER time;
    LARGE_INTEGER freq;

    QueryPerformanceCounter(&time);
    QueryPerformanceFrequency(&freq);

    return (double)time.QuadPart / freq.QuadPart;
}

float Time::GetDeltaTime(){
    return LocalData::GetLocalInst()->deltaTime;
}

void Time::Sleep(float time){
    if (time < 0.0005f)
        return;
    ::Sleep((uint)Round(time * 1000.0f));
}

void TimerManager::CreateTimer(int id, int elapseMillis){
    DebugError("TimerManager::CreateTimer [Unimplemented]");
}

void TimerManager::DestroyTimer(int id){
    DebugError("TimerManager::DestroyTimer [Unimplemented]");
}