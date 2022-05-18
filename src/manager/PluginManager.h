#ifndef __MANAGER_PLUGINMANAGER__
#define __MANAGER_PLUGINMANAGER__

#include <define.h>

#ifdef PLATFORM_WINDOWS
#include <handleapi.h>
#endif

#include <util/List.h>
#include <util/String.h>

namespace simple3deditor {

// 先就试做一下这个功能吧
// 看来有必要将一部分代码做成库，交给插件共享，共通加载同一个动态库

//TODO 待设计
class Plugin : public Object {
private:
#ifdef PLATFORM_WINDOWS
    HMODULE hModule;
#endif

    String name;

public:
    Plugin(String name);
    ~Plugin();

    String GetName();
};

//TODO 待设计
class PluginManager : public Object {
private:
    List<Plugin*> plugins;

public:
    PluginManager();
    ~PluginManager();

    void LoadPlugin(String name);
    bool UnloadPlugin(String name);
    // 计划这里内部的实现以后改为HashMap加速
    Plugin* GetPlugin(String name);
    List<Plugin*>& GetPlugins();
};

}

#endif