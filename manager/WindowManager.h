#ifndef __MANAGER_WINDOWMANAGER__
#define __MANAGER_WINDOWMANAGER__

#include <define.h>

#include <utils/List.h>

class IWindowRegistry : public Object {
public:
    // 窗口名称
    WString name;

    // 获取新窗口，为函数指针主要是避免麻烦的继承
    IWindow*(*NewWindow)();
};

class WindowManager {
private:
    List<IWindowRegistry*> windows;

public:
    WindowManager();
    ~WindowManager();

    void RegisterWindow(WindowRegistry* window);
    void UnregisterWindow(WindowRegistry* window);
    // 计划这里内部的实现以后改为HashMap加速
    WindowRegistry* GetWindow(WString name);
    IWindow* NewWindow(WString name);
    List<IWindowRegistry*>& GetWindows();
};

#endif