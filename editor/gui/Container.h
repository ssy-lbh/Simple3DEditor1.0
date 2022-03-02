#ifndef __EDITOR_GUI_CONTAINER__
#define __EDITOR_GUI_CONTAINER__

#include <define.h>

#include <editor/gui/UIManager.h>

//TODO 容器期望集成选择夹、属性窗口等容器
class LRContainer final : public IWindow {
private:
    IWindow* lWindow;
    IWindow* rWindow;
    int dis = 1;
    IWindow* focus = NULL;
    bool right = false;
    Vector2 size = Vector2(2.0f, 0.0f);
    Vector2 cursorPos;
    bool adjustPos = false;
    bool dragEnable = true;

    // 以下对象用于窗口归并
    SelectionWindow* selWindow = NULL;
    Menu* joinMenu = NULL;

    void InitMenu();

public:
    LRContainer(IWindow* lWindow, IWindow* rWindow);
    LRContainer(IWindow* lWindow, IWindow* rWindow, SelectionWindow* selWindow);
    ~LRContainer();

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnCreate() override;
    virtual void OnClose() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
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
    virtual void OnDropFileA(const char* path) override;
    virtual void OnDropFileW(const wchar_t* path) override;

    void UpdateFocus();
    void FreeWindow();
    IWindow* GetLeftWindow();
    IWindow* GetRightWindow();
    void EnableDrag();
    void DisableDrag();
    bool DragEnabled();
};

class UDContainer final : public IWindow {
private:
    IWindow* uWindow;
    IWindow* dWindow;
    int dis = 1;
    IWindow* focus = NULL;
    bool up = false;
    Vector2 size = Vector2(0.0f, 2.0f);
    Vector2 cursorPos;
    bool adjustPos = false;
    bool dragEnable = true;

    // 以下对象用于窗口归并
    SelectionWindow* selWindow = NULL;
    Menu* joinMenu = NULL;

    void InitMenu();

public:
    UDContainer(IWindow* uWindow, IWindow* dWindow);
    UDContainer(IWindow* uWindow, IWindow* dWindow, SelectionWindow* selWindow);
    virtual ~UDContainer() override;

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnCreate() override;
    virtual void OnClose() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
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
    virtual void OnDropFileA(const char* path) override;
    virtual void OnDropFileW(const wchar_t* path) override;

    void UpdateFocus();
    void FreeWindow();
    IWindow* GetUpWindow();
    IWindow* GetDownWindow();
    void EnableDrag();
    void DisableDrag();
    bool DragEnabled();
};

class SelectionWindow final : public IWindow {
private:
    IWindow* curWindow;
    Menu* selMenu;

    int sizeX, sizeY;
    int cursorX, cursorY;

    void InitMenu();

public:
    SelectionWindow();
    SelectionWindow(IWindow* initialWnd);
    virtual ~SelectionWindow() override;

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnCreate() override;
    virtual void OnClose() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
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
    virtual void OnDropFileA(const char* path) override;
    virtual void OnDropFileW(const wchar_t* path) override;

    IWindow* GetWindow();
    void SetWindow(IWindow* window, bool del = true);
};

#endif