#ifndef __UTIL_TASK__
#define __UTIL_TASK__

#include <define.h>

#include <functional>

namespace simple3deditor {

template <typename T>
class Task final : public Object {
private:
    std::function<T()> task;

public:
    Task<T>(std::function<T()>&& f) : task(f) {}
    ~Task<T>(){}

    T operator()(){ return task(); }
    T Invoke(){ return task(); }
};

template <typename F, typename... U>
auto GetTask(F&& f, U&&... args) -> Task<decltype(f(args...))> {
    return Task<decltype(f(args...))>([=](){ return f(args...); });
}

}

#endif