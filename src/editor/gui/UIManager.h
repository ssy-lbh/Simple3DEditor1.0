#ifndef __EDITOR_GUI_UIMANAGER__
#define __EDITOR_GUI_UIMANAGER__

#include <define.h>

#include <functional>

#include <util/List.h>
#include <util/String.h>
#include <util/math3d/LinearAlgebra.h>

// "����ģʽ"
#include <editor/main/Window.h>
#include <editor/main/Tool.h>
#include <editor/main/Operation.h>

namespace simple3deditor {

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
    // ��Ⱦǰһ����Ҫ����ModelView����
    void Render();
    void RenderWithDepth();
    bool LeftDown();
    bool LeftUp();
    bool RightDown();
    bool RightUp();
    bool Wheel(int delta);
    bool Char(char c);
    bool Unichar(wchar_t c);
    void Foreach(std::function<void(IButton*)> func);

    IButton* GetCurrent();
    IButton* FindCurrent();
};

class IButton : public Object {
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
    // ����OpenGL����Ⱦ�������Ⱦȫ���������ó���ɻ���API
    virtual void OnRender();
    // Ҫ�����ʱOpenGL����������ȾAPI�����Ĵ���
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
    virtual void OnDropFileA(const char* path, uint len);
    virtual void OnDropFileW(const wchar_t* path, uint len);
};

}

#endif