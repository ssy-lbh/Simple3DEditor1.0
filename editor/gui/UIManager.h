#ifndef __EDITOR_GUI_UIMANAGER__
#define __EDITOR_GUI_UIMANAGER__

#include <define.h>

#include <utils/List.h>
#include <utils/String.h>
#include <utils/math3d/LinearAlgebra.h>

// "兼容模式"
#include <editor/main/Window.h>
#include <editor/main/Tool.h>
#include <editor/main/Operation.h>

class UIManager final : public Object {
private:
    List<IButton*> buttons;
    IButton* focus = NULL;
    IButton* cur = NULL;
    Vector2 startPos;
    Vector2 cursorPos;
    bool leftDown = false;
    bool rightDown = false;

public:
    UIManager();
    ~UIManager();

    void CursorMove(Vector2 pos);
    void AddButton(IButton* btn);
    void DeleteButton(IButton* btn);
    // 渲染前一般需要重置ModelView矩阵
    void Render();
    void RenderWithDepth();
    bool LeftDown();
    bool LeftUp();
    bool RightDown();
    bool RightUp();
    bool Wheel(int delta);
    bool Char(char c);
    bool Unichar(wchar_t c);
    void Foreach(void(*func)(IButton*));
    void Foreach(void(*func)(IButton*, void*), void* user);

    IButton* GetCurrent();
    IButton* FindCurrent();
};

interface IButton : public Object {
public:
    IButton();
    virtual ~IButton();

    virtual bool Trigger(Vector2 pos);
    virtual void Hover(Vector2 pos);
    virtual void Click(Vector2 pos);
    virtual void Drag(Vector2 dir);
    virtual void ClickEnd(Vector2 pos, IButton* end);
    virtual void OnFocus(Vector2 pos);
    virtual void OnKillFocus(Vector2 pos, IButton* focus);
    virtual void RightClick(Vector2 pos);
    virtual void RightDrag(Vector2 dir);
    virtual void RightClickEnd(Vector2 pos, IButton* end);
    virtual void OnWheel(int delta);
    virtual void Leave(Vector2 pos);
    virtual bool Char(char c);
    virtual bool Unichar(wchar_t c);
    virtual void Render();
};

class IWindow : public Object {
public:
    IWindow();
    virtual ~IWindow();

    virtual bool IsFocus();
    // 基于OpenGL的渲染，如果渲染全过程已做好抽象可换用API
    virtual void OnRender();
    // 要求调用时OpenGL或者其他渲染API上下文存在
    virtual void OnCreate();
    virtual void OnClose();
    virtual void OnTimer(int id);
    virtual void OnChar(char c);
    virtual void OnUnichar(wchar_t c);
    virtual void OnResize(int x, int y);
    virtual void OnMouseMove(int x, int y);
    virtual void OnLeftDown(int x, int y);
    virtual void OnLeftUp(int x, int y);
    virtual void OnRightDown(int x, int y);
    virtual void OnRightUp(int x, int y);
    virtual void OnFocus();
    virtual void OnKillFocus();
    virtual void OnMouseWheel(int delta);
    virtual void OnMenuAccel(int id, bool accel);
    virtual void OnDropFileA(const char* path);
    virtual void OnDropFileW(const wchar_t* path);
};

#endif