#ifndef __UTILS_OS_THREAD__
#define __UTILS_OS_THREAD__

#include <define.h>

template <typename T, typename... U>
class Task final : public Object {
public:
    struct ArgList {
        U... args;
    };

    T(*func)(U...);
    ArgList list;

    Task(T(*func)(U...), U... args) : func(func), list{args...} {}
    ~Task();

    T Invoke(){
        return func(list.args...);
    }
};

#endif