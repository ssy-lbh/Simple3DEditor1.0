#include <utils/os/Log.h>

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) void __cdecl OnLoad(){
    DebugLog("Plugin Loaded");
}

__declspec(dllexport) void __cdecl OnUnload(){
    DebugLog("Plugin Unloaded");
}

#ifdef __cplusplus
}
#endif
