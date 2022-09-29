#ifndef __UTIL_ITERATOR__
#define __UTIL_ITERATOR__

#include <define.h>

#include <functional>

namespace simple3deditor {

template <typename T>
class IIterator {
public:
    virtual ~IIterator() = 0;

    virtual bool HasNext() = 0;
    virtual T Next() = 0;
    virtual void Remove() = 0;

    // 遍历剩余部分
    virtual void ForEach(std::function<void(T)> func) {
        while (HasNext())
            func(Next());
    }
}

}

#endif