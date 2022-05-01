#ifndef __IO_FACTORY_FACTORYREGISTRY__
#define __IO_FACTORY_FACTORYREGISTRY__

#include <define.h>

#include <util/HashMap.h>
#include <base/factory/Factory.h>

// 这将成为以后多种类的注册点
// 对序列化，取得所有类型窗口，插件注册等都有帮助
template <typename T>
class FactoryRegistry {
protected:
    HashMapA<FFactory<T>> map;

public:
    FactoryRegistry();

    void Register(const char* name, FFactory<T> factory){
        map.Set(name, factory);
    }

    void Unregister(const char* name){
        map.Delete(name);
    }
};

#endif