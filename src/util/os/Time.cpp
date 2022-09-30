#include <util/os/Time.h>

#include <main.h>
#include <util/os/Log.h>
#include <util/math3d/Math.h>

#include <thread>

namespace simple3deditor {

double Time::GetTime(){
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    return (double)ns.count() * 1e-9;
}

float Time::GetDeltaTime(){
    return LocalData::GetLocalInst()->deltaTime;
}

void Time::Sleep(float time){
    std::this_thread::sleep_for(std::chrono::nanoseconds((uint64_t)Round(time * 1e9f)));
}

}