#ifndef __UTIL_COLLECTION__
#define __UTIL_COLLECTION__

#include <define.h>

#include <functional>

#include <util/Iterable.h>

namespace simple3deditor {

template <typename T>
class ICollection : public IIterable<T> {
public:
    virtual ~ICollection() = 0;

    virtual size_t Size() = 0;
    virtual bool Empty() = 0;
    virtual bool Contains(T o) = 0;
    // 返回后需要delete[]
    virtual T[] ToArray() = 0;
    virtual T[] ToArray(T[] a) = 0;
    virtual bool Add(T o) = 0;
    virtual bool Remove(T o) = 0;
}

}

#endif