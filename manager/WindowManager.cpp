#include <manager/WindowManager.h>

#include <utils/String.h>

WindowManager::WindowManager(){}

WindowManager::~WindowManager(){
    Free(windows);
}

void WindowManager::RegisterWindow(WindowRegistry* window){
    windows.Add(window);
}

void WindowManager::UnregisterWindow(WindowRegistry* window){
    windows.Remove(window);
}

WindowRegistry* WindowManager::UnregisterWindow(WString name){
    WindowRegistry* window = GetWindow(name);
    UnregisterWindow(window);
    return window;
}

WindowRegistry* WindowManager::GetWindow(WString name){
    size_t len = windows.Size();
    for (size_t i = 0; i < len; i++)
        if (windows[i]->name == name)
        return windows[i];
    return NULL;
}

IWindow* WindowManager::NewWindow(WString name){
    size_t len = windows.Size();
    for (size_t i = 0; i < len; i++)
        if (windows[i]->name == name)
        return windows[i]->NewWindow();
    return NULL;
}

List<WindowRegistry*>& WindowManager::GetWindows(){
    return windows;
}