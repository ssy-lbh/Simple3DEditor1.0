#ifndef __UTILS_UTILS__
#define __UTILS_UTILS__

#include <define.h>

#include <exception>

template <typename T, typename Tp>
bool InstanceOf(Tp* o){
    try{
        dynamic_cast<T*>(o);
    }catch(const std::bad_cast& e){
        return false;
    }
    return true;
}

#endif