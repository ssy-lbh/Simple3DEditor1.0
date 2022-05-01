#ifndef __EDITOR_GUI_CONTAINER__
#define __EDITOR_GUI_CONTAINER__

#include <define.h>

#include <editor/main/Window.h>

//TODO 容器期望集成选择夹、属性窗口等容器
class LRContainer final : public AWindow {
private:
    AWindow* lWindow;
    AWindow* rWindow;
    int dis = 1;
    AWindow* focusWindow = NULL;
    bool right = false;
    bool adjustPos = false;
    bool dragEnable = true;

    // 以下对象用于窗口归并
    SelectionWindow* selWindow = NULL;
    Menu* joinMenu = NULL;

    void InitMenu();

public:
    LRContainer(AWindow* lWindow, AWindow* rWindow);
    LRContainer(AWindow* lWindow, AWindow* rWindow, SelectionWindow* selWindow);
    ~LRContainer();

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path) override;
    virtual void OnDropFileW(const wchar_t* path) override;

    void UpdateFocus();
    void FreeWindow();
    AWindow* GetLeftWindow();
    AWindow* GetRightWindow();
    void EnableDrag();
    void DisableDrag();
    bool DragEnabled();
};

class UDContainer final : public AWindow {
private:
    AWindow* uWindow;
    AWindow* dWindow;
    int dis = 1;
    AWindow* focusWindow = NULL;
    bool up = false;
    bool adjustPos = false;
    bool dragEnable = true;

    // 以下对象用于窗口归并
    SelectionWindow* selWindow = NULL;
    Menu* joinMenu = NULL;

    void InitMenu();

public:
    UDContainer(AWindow* uWindow, AWindow* dWindow);
    UDContainer(AWindow* uWindow, AWindow* dWindow, SelectionWindow* selWindow);
    virtual ~UDContainer() override;

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path) override;
    virtual void OnDropFileW(const wchar_t* path) override;

    void UpdateFocus();
    void FreeWindow();
    AWindow* GetUpWindow();
    AWindow* GetDownWindow();
    void EnableDrag();
    void DisableDrag();
    bool DragEnabled();
};

class SelectionWindow final : public AWindow {
private:
    AWindow* curWindow;
    Menu* selMenu;

    void InitMenu();

public:
    SelectionWindow();
    SelectionWindow(AWindow* initialWnd);
    virtual ~SelectionWindow() override;

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path) override;
    virtual void OnDropFileW(const wchar_t* path) override;

    AWindow* GetWindow();
    void SetWindow(AWindow* window, bool del = true);
};

#endif