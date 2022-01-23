#ifndef __UIMGR__
#define __UIMGR__

#include <stddef.h>
#include <windef.h>

#include "list.h"
#include "vecmath.h"

class UIManager;
class ViewportManager;

class IButton;
class IOperation;
class ITool;

class IWindow;
//class IContainer;
//TODO 此接口期望集成选择夹、拆分窗口、属性窗口等容器

class UIManager {
private:
    List<IButton*> buttons;
    IButton* cur = NULL;
    Vector2 startPos;
    Vector2 cursorPos;
    bool leftDown = false;
public:
    UIManager();
    ~UIManager();
    void CursorMove(Vector2 pos);
    void AddButton(IButton* btn);
    void DeleteButton(IButton* btn);
    void Render(float aspect);
    void Render();
    void RenderRaw();
    void RenderTransform(float aspect);
    void RenderTransform();
    bool LeftDown();
    bool LeftUp();
};

class ViewportManager {
private:
    List<RECT> rects;
    RECT curRect;
public:
    static ViewportManager* inst;

    ViewportManager();
    ~ViewportManager();
    void Reset(HWND hWnd);
    void SetViewport(RECT rect);
    void PushViewport(RECT rect);
    void PopViewport();
    RECT GetCurrentRect();
    LONG GetCurrentWidth();
    LONG GetCurrentHeight();
    float GetAspect();
    void EnableScissor();
    void DisableScissor();
    Vector2 GetClientSize();
};

//TODO 按钮在触发按下后持续生效到停止
class IButton {
public:
    IButton();
    virtual ~IButton();

    virtual bool Trigger(Vector2 pos);
    virtual void Hover();
    virtual void Click();
    virtual void Drag(Vector2 dir);
    virtual void ClickEnd();
    virtual void Leave();
    virtual void Render();
};

class IOperation {
public:
    IOperation();
    virtual ~IOperation();

    virtual void OnEnter();
    virtual void OnConfirm();
    virtual void OnUndo();
    virtual void OnRightDown();
    virtual void OnRightUp();
    virtual void OnMove();
    virtual void OnCommand(UINT id);
};

class ITool {
public:
    ITool();
    virtual ~ITool();

    virtual void OnSelect();
    virtual void OnUnselect();
    virtual void OnLeftDown();
    virtual void OnLeftUp();
    virtual void OnRightDown();
    virtual void OnRightUp();
    virtual void OnMove();
    virtual void OnRender();
};

class IWindow {
public:
    IWindow();
    virtual ~IWindow();

    virtual void SetFrame(HWND hWnd);
    virtual bool IsFocus();
    virtual void OnRender();
    virtual void OnCreate();
    virtual void OnClose();
    virtual void OnResize(int x, int y);
    virtual void OnMouseMove(int x, int y);
    virtual void OnLeftDown(int x, int y);
    virtual void OnLeftUp(int x, int y);
    virtual void OnRightDown(int x, int y);
    virtual void OnRightUp(int x, int y);
    virtual void OnMouseHover(int key, int x, int y);
    virtual void OnMouseLeave();
    virtual void OnFocus();
    virtual void OnKillFocus();
    virtual void OnMouseWheel(int delta);
    virtual void OnMenuAccel(int id, bool accel);
    virtual void OnControl(int inform, int id, HWND hctl);
};

#endif