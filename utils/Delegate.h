#ifndef __UTILS_DELEGATE__
#define __UTILS_DELEGATE__

#include <define.h>

#include <utils/List.h>

template <typename T, typename... U>
class Delegate {
public:
    typedef T(*func_t)(U...);

private:
    List<func_t> funcs;

public:
    Delegate<T, U...>() : funcs() {}
    Delegate<T, U...>(size_t size) : funcs(size) {}
    Delegate<T, U...>(Delegate<T, U...>&& delegate) : funcs(delegate.funcs) {}
    Delegate<T, U...>(const Delegate<T, U...>& delegate) : funcs(delegate.funcs) {}
    Delegate<T, U...>& operator=(Delegate<T, U...>&& delegate){ funcs = delegate.funcs; return *this; }
    Delegate<T, U...>& operator=(const Delegate<T, U...>& delegate){ funcs = delegate.funcs; return *this; }
    ~Delegate<T, U...>(){}
    Delegate<T, U...>& operator+=(func_t f){ funcs += f; return *this; }
    Delegate<T, U...>& operator-=(func_t f){ funcs -= f; return *this; }
    Delegate<T, U...>& operator!(){ return !funcs; }

    Delegate<T, U...>& operator()(U... params){
        size_t size = funcs.Size();
        for (size_t i = 0; i < size; i++){
            funcs[i](params...);
        }
    }

    size_t Size(){ return funcs.Size(); }
    void Clear(){ funcs.Clear(); }
    void Clear(size_t memory){ funcs.Clear(memory); }
    func_t& operator[](size_t index){ return funcs[index]; }
};

#endif