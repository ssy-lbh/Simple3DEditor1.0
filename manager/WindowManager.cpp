#include <manager/WindowManager.h>

#include <utils/String.h>

WindowManager::WindowManager(){}
WindowManager::~WindowManager(){}

void WindowManager::RegisterWindow(WindowRegistry* window){
    windows.Add(window);
}

void WindowManager::UnregisterWindow(WindowRegistry* window){
    windows.Remove(window);
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

List<IWindowRegistry*>& WindowManager::GetWindows(){
    return windows;
}