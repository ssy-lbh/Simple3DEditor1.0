#ifndef __MANAGER_WINDOWMANAGER__
#define __MANAGER_WINDOWMANAGER__

#include <define.h>

#include <utils/List.h>

class IWindowRegistry : public Object {
public:
    // 获取窗口名称
    WString GetName();
    // 获取新窗口
    IWindow* NewWindow();
};

class WindowManager {
private:
    List<IWindowRegistry*> windows;

public:
    WindowManager();
    ~WindowManager();

    void RegisterWindow(WindowRegistry* window);
    void UnregisterWindow(WindowRegistry* window);
    WindowRegistry* GetWindow();
    IWindow* NewWindow();
};

#endif