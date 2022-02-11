#ifndef __UIMGR__
#define __UIMGR__

#include "define.h"

#include <windef.h>

#include "list.h"
#include "vecmath.h"
#include "gltools.h"

class UIManager;
class ViewportManager;

class IButton;
class IOperation;
class ITool;

class IWindow;

class UIManager : public Object {
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
    bool Char(char c);
    bool Unichar(wchar_t c);
    void Foreach(void(*func)(IButton*));
    void Foreach(void(*func)(IButton*, void*), void* user);
};

class ViewportManager : public Object {
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
    RECT CalculateChildRect(float left, float right, float bottom, float top);
    void PushChildViewport(float left, float right, float bottom, float top);
};

//TODO 按钮在触发按下后持续生效到停止
class IButton : public Object {
public:
    IButton();
    virtual ~IButton();

    virtual bool Trigger(Vector2 pos);
    virtual void Hover();
    virtual void Click();
    virtual void Drag(Vector2 dir);
    virtual void ClickEnd();
    virtual void Leave();
    virtual bool Char(char c);
    virtual bool Unichar(wchar_t c);
    virtual void Render();
};

class IconButton : public IButton {
private:
    Vector2 position;
    Vector2 size;
    float radius;

    GLTexture2D* texture = NULL;
    void(*onClick)(void*) = NULL;
    void* user = NULL;

public:
    IconButton(Vector2 position, Vector2 size);
    IconButton(Vector2 position, Vector2 size, float radius);
    virtual ~IconButton() override;

    virtual bool Trigger(Vector2 pos) override;
    virtual void Click() override;
    virtual void Render() override;

    void OnClick(void(*func)(void*));
    void SetUserData(void* data);
    void SetIcon(const char* texPath);
    void SetIcon(int iconRes);
    void SetIcon(GLTexture2D* tex);
};

class UIEditA : public IButton {
private:
    // 左上角位置
    Vector2 position;
    Vector2 size;
    Vector3 bkColor = Vector3::zero;
    Vector3 fontColor = Vector3::one;
    Vector3 selColor = Vector3(0.0f, 0.0f, 1.0f);

    char text[MAX_PATH + 1];
    bool editing = false;
    size_t editPos;

public:
    UIEditA(Vector2 pos, Vector2 size);
    virtual ~UIEditA();

    virtual bool Trigger(Vector2 pos) override;
    virtual void Hover() override;
    virtual void Click() override;
    virtual void Leave() override;
    virtual bool Char(char c) override;
    virtual void Render() override;

    void SetBackgroundColor(Vector3 color);
    void SetFontColor(Vector3 color);
    void SetSelectionColor(Vector3 color);
    Vector3 GetBackgroundColor();
    Vector3 GetFontColor();
    Vector3 GetSelectionColor();
};

class IOperation : public Object {
public:
    IOperation();
    virtual ~IOperation();

    virtual void OnEnter();
    virtual void OnConfirm();
    virtual void OnUndo();
    virtual void OnRightDown();
    virtual void OnRightUp();
    virtual void OnMove();
    virtual void OnCommand(int id);
};

class ITool : public Object {
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
    virtual void OnCommand(int id);
};

class IWindow : public Object {
public:
    IWindow();
    virtual ~IWindow();

    virtual bool IsFocus();
    virtual void OnRender();
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
    virtual void OnMouseHover(int key, int x, int y);
    virtual void OnMouseLeave();
    virtual void OnFocus();
    virtual void OnKillFocus();
    virtual void OnMouseWheel(int delta);
    virtual void OnMenuAccel(int id, bool accel);
    virtual void OnDropFileA(const char* path);
    virtual void OnDropFileW(const wchar_t* path);
};

#endif