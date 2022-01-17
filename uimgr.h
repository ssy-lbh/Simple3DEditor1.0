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
    void Render(float aspect);
    bool LeftDown();
    bool LeftUp();
};

class ViewportManager {
private:
    List<RECT> rects;
    RECT curRect;
public:
    ViewportManager();
    ~ViewportManager();
    void Reset(HWND hWnd);
    void PushViewport(RECT rect);
    void PopViewport();
    LONG GetCurrentWidth();
    LONG GetCurrentHeight();
    float GetAspect();
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

#endif