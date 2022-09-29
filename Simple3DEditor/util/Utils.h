#ifndef __UTIL_UTILS__
#define __UTIL_UTILS__

#include <define.h>

#include <typeinfo>
#include <exception>

namespace simple3deditor {

template <typename T, typename Tp>
inline bool InstanceOf(Tp* o){
    return typeid(*o) == typeid(T);
}

template <typename T, typename Tp>
inline bool ChildOf(Tp* o){
    try{
        dynamic_cast<T*>(o);
    }catch(const std::bad_cast& e){
        return false;
    }
    return true;
}

}

#endif