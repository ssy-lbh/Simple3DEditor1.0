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
    Delegate<T, U...>(Delegate<T, U...>&& d) : funcs(d.funcs) {}
    Delegate<T, U...>(const Delegate<T, U...>& d) : funcs(d.funcs) {}
    Delegate<T, U...>& operator=(Delegate<T, U...>&& d){ funcs = d.funcs; return *this; }
    Delegate<T, U...>& operator=(const Delegate<T, U...>& d){ funcs = d.funcs; return *this; }
    ~Delegate<T, U...>(){}
    Delegate<T, U...>& operator+=(func_t f){ funcs += f; return *this; }
    Delegate<T, U...>& operator+=(Delegate<T, U...>& d){ funcs += d.funcs; return *this; }
    Delegate<T, U...>& operator-=(func_t f){ funcs -= f; return *this; }
    Delegate<T, U...>& operator-=(Delegate<T, U...>& d){ funcs -= d.funcs; return *this; }
    Delegate<T, U...>& operator!(){ return !funcs; }

    Delegate<T, U...>& Add(func_t f){ funcs += f; return *this; }
    Delegate<T, U...>& Remove(func_t f){ funcs -= f; return *this; }

    Delegate<T, U...>& operator()(U... params){
        size_t size = funcs.Size();
        for (size_t i = 0; i < size; i++){
            funcs[i](params...);
        }
        return *this;
    }

    size_t Size(){ return funcs.Size(); }
    void Clear(){ funcs.Clear(); }
    void Clear(size_t memory){ funcs.Clear(memory); }
    func_t& operator[](size_t index){ return funcs[index]; }

    List<func_t>& GetFuncs(){ return funcs; }
};

template <typename T, typename D, typename... U>
class DelegateWithData {
public:
    typedef T(*func_t)(D, U...);

    struct Func {
        func_t func;
        D data;
    };

private:
    List<Func> funcs;

public:
    DelegateWithData<T, D, U...>() : funcs() {}
    DelegateWithData<T, D, U...>(size_t size) : funcs(size) {}
    DelegateWithData<T, D, U...>(DelegateWithData<T, D, U...>&& d) : funcs(d.funcs) {}
    DelegateWithData<T, D, U...>(const DelegateWithData<T, D, U...>& d) : funcs(d.funcs) {}
    DelegateWithData<T, D, U...>& operator=(DelegateWithData<T, D, U...>&& d){ funcs = d.funcs; return *this; }
    DelegateWithData<T, D, U...>& operator=(const DelegateWithData<T, D, U...>& d){ funcs = d.funcs; return *this; }
    ~DelegateWithData<T, D, U...>(){}
    DelegateWithData<T, D, U...>& operator+=(func_t f){ return Add(f); }
    DelegateWithData<T, D, U...>& operator+=(DelegateWithData<T, D, U...>& d){ funcs += d.funcs; return *this; }
    DelegateWithData<T, D, U...>& operator-=(func_t f){ Remove(f); return *this; }
    DelegateWithData<T, D, U...>& operator-=(DelegateWithData<T, D, U...>& d){ funcs -= d.funcs; return *this; }
    DelegateWithData<T, D, U...>& operator!(){ return !funcs; }

    DelegateWithData<T, D, U...>& Add(func_t f){
        Func func;
        func.func = f;
        funcs += func;
        return *this;
    }

    DelegateWithData<T, D, U...>& Add(func_t f, D d){
        Func func;
        func.func = f;
        func.data = d;
        funcs += func;
        return *this;
    }

    bool Remove(func_t f){
        size_t size = funcs.Size();
        for (size_t i = 0; i < size; i++){
            if (funcs[i].func == f){
                funcs.RemoveAt(i);
                return true;
            }
        }
        return false;
    }

    DelegateWithData<T, D, U...>& operator()(U... params){
        size_t size = funcs.Size();
        for (size_t i = 0; i < size; i++){
            funcs[i].func(funcs[i].data, params...);
        }
        return *this;
    }

    size_t Size(){ return funcs.Size(); }
    void Clear(){ funcs.Clear(); }
    void Clear(size_t memory){ funcs.Clear(memory); }
    func_t& operator[](size_t index){ return funcs[index].func; }

    List<Func>& GetFuncs(){ return funcs; }
};

template <typename T, typename... U>
class DelegateWithData<T, void*, U...> {
public:
    typedef T(*func_t)(void*, U...);

    struct Func {
        func_t func;
        void* data;
    };

private:
    List<Func> funcs;

public:
    DelegateWithData<T, void*, U...>() : funcs() {}
    DelegateWithData<T, void*, U...>(size_t size) : funcs(size) {}
    DelegateWithData<T, void*, U...>(DelegateWithData<T, void*, U...>&& d) : funcs(d.funcs) {}
    DelegateWithData<T, void*, U...>(const DelegateWithData<T, void*, U...>& d) : funcs(d.funcs) {}
    DelegateWithData<T, void*, U...>& operator=(DelegateWithData<T, void*, U...>&& d){ funcs = d.funcs; return *this; }
    DelegateWithData<T, void*, U...>& operator=(const DelegateWithData<T, void*, U...>& d){ funcs = d.funcs; return *this; }
    ~DelegateWithData<T, void*, U...>(){}
    template <typename D>
    DelegateWithData<T, void*, U...>& operator+=(T(*f)(D*, U...)){ return Add(f); }
    DelegateWithData<T, void*, U...>& operator+=(DelegateWithData<T, void*, U...>& d){ funcs += d.funcs; return *this; }
    template <typename D>
    DelegateWithData<T, void*, U...>& operator-=(T(*f)(D*, U...)){ Remove(f); return *this; }
    DelegateWithData<T, void*, U...>& operator-=(DelegateWithData<T, void*, U...>& d){ funcs -= d.funcs; return *this; }
    DelegateWithData<T, void*, U...>& operator!(){ return !funcs; }

    template <typename D>
    DelegateWithData<T, void*, U...>& Add(T(*f)(D*, U...)){
        Func func;
        func.func = (func_t)f;
        funcs += func;
        return *this;
    }

    template <typename D>
    DelegateWithData<T, void*, U...>& Add(T(*f)(D*, U...), D* d){
        Func func;
        func.func = (func_t)f;
        func.data = (void*)d;
        funcs += func;
        return *this;
    }

    template <typename D>
    bool Remove(T(*f)(D*, U...)){
        size_t size = funcs.Size();
        for (size_t i = 0; i < size; i++){
            if (funcs[i].func == (func_t)f){
                funcs.RemoveAt(i);
                return true;
            }
        }
        return false;
    }

    DelegateWithData<T, void*, U...>& operator()(U... params){
        size_t size = funcs.Size();
        for (size_t i = 0; i < size; i++){
            funcs[i].func(funcs[i].data, params...);
        }
        return *this;
    }

    size_t Size(){ return funcs.Size(); }
    void Clear(){ funcs.Clear(); }
    void Clear(size_t memory){ funcs.Clear(memory); }
    func_t& operator[](size_t index){ return funcs[index].func; }

    List<Func>& GetFuncs(){ return funcs; }
};

template <typename T, typename C, typename... U>
class DelegateWithInst {
public:
    typedef T(C::*func_t)(U...);

    struct Func {
        func_t func;
        C* inst;
    };

private:
    List<Func> funcs;

public:
    DelegateWithInst<T, C, U...>() : funcs() {}
    DelegateWithInst<T, C, U...>(size_t size) : funcs(size) {}
    DelegateWithInst<T, C, U...>(DelegateWithInst<T, C, U...>&& d) : funcs(d.funcs) {}
    DelegateWithInst<T, C, U...>(const DelegateWithInst<T, C, U...>& d) : funcs(d.funcs) {}
    DelegateWithInst<T, C, U...>& operator=(DelegateWithInst<T, C, U...>&& d){ funcs = d.funcs; return *this; }
    DelegateWithInst<T, C, U...>& operator=(const DelegateWithInst<T, C, U...>& d){ funcs = d.funcs; return *this; }
    ~DelegateWithInst<T, C, U...>(){}
    DelegateWithInst<T, C, U...>& operator+=(DelegateWithInst<T, C, U...>& d){ funcs += d.funcs; return *this; }
    DelegateWithInst<T, C, U...>& operator-=(func_t f){ Remove(f); return *this; }
    DelegateWithInst<T, C, U...>& operator-=(DelegateWithInst<T, C, U...>& d){ funcs -= d.funcs; return *this; }
    DelegateWithInst<T, C, U...>& operator!(){ return !funcs; }

    DelegateWithInst<T, C, U...>& Add(func_t f, C* inst){
        Func func;
        func.func = f;
        func.inst = inst;
        funcs += func;
        return *this;
    }

    bool Remove(func_t f){
        size_t size = funcs.Size();
        for (size_t i = 0; i < size; i++){
            if (funcs[i].func == f){
                funcs.RemoveAt(i);
                return true;
            }
        }
        return false;
    }

    DelegateWithInst<T, C, U...>& operator()(U... params){
        size_t size = funcs.Size();
        for (size_t i = 0; i < size; i++){
            funcs[i].inst->*funcs[i].func(params...);
        }
        return *this;
    }

    size_t Size(){ return funcs.Size(); }
    void Clear(){ funcs.Clear(); }
    void Clear(size_t memory){ funcs.Clear(memory); }
    func_t& operator[](size_t index){ return funcs[index].func; }

    List<Func>& GetFuncs(){ return funcs; }
};

#endif