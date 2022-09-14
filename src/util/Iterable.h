#ifndef __UTIL_ITERABLE__
#define __UTIL_ITERABLE__

#include <define.h>

#include <functional>

#include <util/Iterator.h>

namespace simple3deditor {

template <typename T>
class IIterable {
public:
    virtual ~IIterable() = 0;

    // 用完需delete
    IIterator<T>* Iterator() = 0;

    virtual void ForEach(std::function<void(T)> func) {
        IIterator<T>* i = Iterator();
        while (i->HasNext())
            func(i->Next());
        delete i;
    }
}

}

#endif