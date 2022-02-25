#include "timetools.h"

#include <windows.h>

#include "main.h"

float TimeUtils::GetTime(){
    LARGE_INTEGER time;
    LARGE_INTEGER freq;

    QueryPerformanceCounter(&time);
    QueryPerformanceFrequency(&freq);

    return (double)time.QuadPart / freq.QuadPart;
}

void TimerManager::CreateTimer(int id, int elapseMillis){
    DebugError("TimerManager::CreateTimer [Unimplemented]");
    //SetTimer(Main::hWnd, id, elapseMillis, NULL);
}

void TimerManager::DestroyTimer(int id){
    DebugError("TimerManager::DestroyTimer [Unimplemented]");
    //KillTimer(Main::hWnd, id);
}