#ifndef __UTILS_DELEGATE__
#define __UTILS_DELEGATE__

#include <define.h>

#include <functional>

#include <utils/List.h>

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

#endif