#ifndef __MANAGER_PLUGINMANAGER__
#define __MANAGER_PLUGINMANAGER__

#include <define.h>

#include <utils/List.h>

// 先就试做一下这个功能吧
// 看来有必要将一部分代码做成库，交给插件共享，共通加载同一个动态库

//TODO 待设计
class Plugin : public Object {
private:
    handle hModule;

    WString name;

public:
    Plugin(WString name);
    ~Plugin();
};

//TODO 待设计
class PluginManager : public Object {
private:
    List<Plugin*> plugins;

public:
    PluginManager();
    ~PluginManager();

    void LoadPlugin(WString name);
    void UnloadPlugin(WString name);
    Plugin* GetPlugin(WString name);
    List<Plugin*>& GetPlugins();
};

#endif