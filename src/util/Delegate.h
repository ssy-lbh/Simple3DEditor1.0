#ifndef __UTIL_DELEGATE__
#define __UTIL_DELEGATE__

#include <define.h>

#include <functional>

#include <util/List.h>

namespace simple3deditor {

template <typename... U>
class Delegate {
private:
    List<std::function<void(U...)>> funcs;

public:
    Delegate<U...>() : funcs() {}
    Delegate<U...>(size_t size) : funcs(size) {}
    Delegate<U...>(Delegate<U...>&& d) : funcs(d.funcs) {}
    Delegate<U...>(const Delegate<U...>& d) : funcs(d.funcs) {}
    Delegate<U...>& operator=(Delegate<U...>&& d){ funcs = d.funcs; return *this; }
    Delegate<U...>& operator=(const Delegate<U...>& d){ funcs = d.funcs; return *this; }
    ~Delegate<U...>(){}
    Delegate<U...>& operator+=(std::function<void(U...)> f){ funcs += f; return *this; }
    Delegate<U...>& operator+=(Delegate<U...>& d){ funcs += d.funcs; return *this; }
    Delegate<U...>& operator-=(std::function<void(U...)> f){ funcs -= f; return *this; }
    Delegate<U...>& operator-=(Delegate<U...>& d){ funcs -= d.funcs; return *this; }
    Delegate<U...>& operator!(){ return !funcs; }

    // 尤其要注意一下是否可行，可能实现有问题
    template <typename T>
    Delegate<U...>& operator-=(T t){
        for (size_t i = 0; i < funcs.Size(); i++)
            if (funcs[i].template target<T>() == t)
                funcs.RemoveAt(i);
        return *this;
    }

    Delegate<U...>& Add(std::function<void(U...)>&& f){ funcs += f; return *this; }

    Delegate<U...>& operator()(U... params){
        size_t size = funcs.Size();
        for (size_t i = 0; i < size; i++)
            funcs[i](params...);
        return *this;
    }

    size_t Size(){ return funcs.Size(); }
    void Clear(){ funcs.Clear(); }
    void Clear(size_t memory){ funcs.Clear(memory); }
    std::function<void(U...)>& operator[](size_t index){ return funcs[index]; }
};

}

#endif