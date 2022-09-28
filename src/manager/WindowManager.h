#ifndef __MANAGER_WINDOWMANAGER__
#define __MANAGER_WINDOWMANAGER__

#include <define.h>

#include <util/List.h>
#include <util/String.h>

namespace simple3deditor {

class WindowRegistry : public Object {
public:
    // 窗口名称
    WString name;

    // 获取新窗口
    IWindow*(*NewWindow)();
};

class WindowManager {
private:
    List<WindowRegistry*> windows;

public:
    WindowManager();
    ~WindowManager();

    void RegisterWindow(WindowRegistry* window);
    void UnregisterWindow(WindowRegistry* window);
    WindowRegistry* UnregisterWindow(WString name);
    // 计划这里内部的实现以后改为HashMap加速
    WindowRegistry* GetWindow(WString name);
    IWindow* NewWindow(WString name);
    List<WindowRegistry*>& GetWindows();
};

}

#endif