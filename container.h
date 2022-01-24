#ifndef __CONTAINER__
#define __CONTAINER__

#include "uimgr.h"

//TODO 容器期望集成选择夹、拆分窗口、属性窗口等容器

class LRContainer : public IWindow {
private:
    IWindow* lWindow;
    IWindow* rWindow;
    LONG dis = 800;
    IWindow* focus = NULL;
    bool right = false;
    Vector2 size;
    Vector2 cursorPos;
    bool adjustPos = false;

public:
    LRContainer(IWindow* lWindow, IWindow* rWindow);
    ~LRContainer();

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnCreate(HWND hWnd) override;
    virtual void OnClose() override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMouseHover(int key, int x, int y) override;
    virtual void OnMouseLeave() override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;

    void UpdateFocus();
    void FreeWindow();
    IWindow* GetLeftWindow();
    IWindow* GetRightWindow();
};

class UDContainer : public IWindow {
private:
    IWindow* uWindow;
    IWindow* dWindow;
    LONG dis;
    IWindow* focus = NULL;
    bool right = false;
    Vector2 size;
    Vector2 cursorPos;
    bool adjustPos = false;

public:
    UDContainer(IWindow* uWindow, IWindow* dWindow);
    virtual ~UDContainer() override;

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnCreate(HWND hWnd) override;
    virtual void OnClose() override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMouseHover(int key, int x, int y) override;
    virtual void OnMouseLeave() override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;

    void UpdateFocus();
    void FreeWindow();
    IWindow* GetUpWindow();
    IWindow* GetRightWindow();
};

#endif