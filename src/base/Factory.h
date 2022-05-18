#ifndef __BASE_FACTORY__
#define __BASE_FACTORY__

#include <define.h>

#include <functional>

#include <util/HashMap.h>

namespace simple3deditor {

template <typename T>
using FFactory = std::function<T*()>;

// 这将成为以后多种类的注册点
// 对序列化，取得所有类型窗口，插件注册等都有帮助
template <typename T>
class FactoryRegistryA {
protected:
    HashMapA<FFactory<T>> map;

public:
    FactoryRegistryA(){}
    ~FactoryRegistryA(){}

    void Register(const char* name, FFactory<T> factory){
        map.Set(name, factory);
    }

    template <typename U>
    void Register(const char* name){
        map.Set(name, []{ return new U(); });
    }

    void Unregister(const char* name){
        map.Delete(name);
    }

    T* New(const char* name){
        return map.Get(name)();
    }

    FFactory<T> GetFactory(const char* name){
        map.Get(name);
    }

    void Foreach(std::function<void(const String&, FFactory<T>)> func){
        map.Foreach(func);
    }
};

template <typename T>
class FactoryRegistryW {
protected:
    HashMapW<FFactory<T>> map;

public:
    FactoryRegistryW(){}
    ~FactoryRegistryW(){}

    void Register(const wchar_t* name, FFactory<T> factory){
        map.Set(name, factory);
    }

    template <typename U>
    void Register(const wchar_t* name){
        map.Set(name, []{ return new U(); });
    }

    void Unregister(const wchar_t* name){
        map.Delete(name);
    }

    T* New(const wchar_t* name){
        return map.Get(name)();
    }

    FFactory<T> GetFactory(const wchar_t* name){
        map.Get(name);
    }

    void Foreach(std::function<void(const WString&, FFactory<T>)> func){
        map.Foreach(func);
    }
};

}

#endif