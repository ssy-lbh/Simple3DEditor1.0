#include <manager/PluginManager.h>

#include <windows.h>

#include <util/StringBuilder.h>
#include <io/File.h>
#include <util/os/Log.h>

Plugin::Plugin(String name) : name(name) {
    StringBuilderA path;

    if (File::IsSpecial(name)){
        DebugError("[Security Warning] Plugin Name \"%s\" Has Special Chars", name.GetString());
    }

    path += L".\\plugins\\";
    path += name;
    path += L".dll";

    hModule = LoadLibraryA(path.GetString());
    if (!hModule){
        DebugError("Plugin At Path \"%s\" Not Found", path.GetString());
        return;
    }

    // 初始化函数调用
    void(*onLoad)() = (void(*)())GetProcAddress(hModule, "OnLoad");
    if (onLoad){
        onLoad();
    }else{
        DebugLog("Plugin \"%s\" Function OnLoad Not Found", name.GetString());
    }
}

Plugin::~Plugin(){
    void(*onUnload)() = (void(*)())GetProcAddress(hModule, "OnUnload");
    if (onUnload){
        onUnload();
    }else{
        DebugLog("Plugin \"%s\" Function OnLoad Not Found", name.GetString());
    }

    FreeLibrary(hModule);
}

String Plugin::GetName(){
    return name;
}

PluginManager::PluginManager(){}

PluginManager::~PluginManager(){
    Free(plugins);
}

void PluginManager::LoadPlugin(String name){
    plugins.Add(new Plugin(name));
}

bool PluginManager::UnloadPlugin(String name){
    size_t size = plugins.Size();
    for (size_t i = 0; i < size; i++){
        if (plugins[i]->GetName() == name){
            plugins.RemoveAt(i);
            return true;
        }
    }
    return false;
}

Plugin* PluginManager::GetPlugin(String name){
    size_t size = plugins.Size();
    for (size_t i = 0; i < size; i++){
        if (plugins[i]->GetName() == name){
            return plugins[i];
        }
    }
    return NULL;
}

List<Plugin*>& PluginManager::GetPlugins(){
    return plugins;
}