#ifndef __EDITOR_GUI_UIMANAGER__
#define __EDITOR_GUI_UIMANAGER__

#include <define.h>

#include <utils/List.h>
#include <utils/String.h>
#include <utils/math3d/LinearAlgebra.h>

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

class IconButton final : public IButton {
private:
    Vector2 position;
    Vector2 size;
    float radius;

    GLTexture2D* texture = NULL;
    void(*onClick)(void*) = NULL;
    void* userData = NULL;

public:
    IconButton(Vector2 position, Vector2 size);
    IconButton(Vector2 position, Vector2 size, float radius);
    virtual ~IconButton() override;

    virtual bool Trigger(Vector2 pos) override;
    virtual void Click(Vector2 pos) override;
    virtual void Render() override;

    void OnClick(void(*func)(void*));
    void SetUserData(void* data);
    void SetIcon(const char* texPath);
    void SetIcon(int iconRes);
    void SetIcon(GLTexture2D* tex);
};

class UIEditA final : public IButton {
private:
    // 左上角位置
    Vector2 position;
    Vector2 size;
    // 意为半径占高度的比例
    float radius = 0.0f;
    Vector3 bkColor = Vector3::zero;
    Vector3 fontColor = Vector3::one;
    Vector3 selColor = Vector3(0.0f, 0.0f, 1.0f);

    char text[DEFAULT_STRING_LENGTH + 1];
    bool editing = false;
    size_t editPos;

    void(*onEdit)(char*, void*) = NULL;
    void* userData = NULL;

public:
    UIEditA(Vector2 pos, float width);
    UIEditA(Vector2 pos, float width, void(*onEdit)(char*, void*));
    UIEditA(Vector2 pos, float width, void(*onEdit)(char*, void*), void* userData);
    virtual ~UIEditA();

    virtual bool Trigger(Vector2 pos) override;
    virtual void Hover(Vector2 pos) override;
    virtual void Click(Vector2 pos) override;
    virtual void Leave(Vector2 pos) override;
    virtual bool Char(char c) override;
    virtual void Render() override;

    char* GetText();
    size_t GetTextMaxLength();

    void SetBackgroundColor(Vector3 color);
    void SetFontColor(Vector3 color);
    void SetSelectionColor(Vector3 color);
    void SetCornerRadius(float radius);
    Vector3 GetBackgroundColor();
    Vector3 GetFontColor();
    Vector3 GetSelectionColor();
    float GetCornerRadius();
};

class UIEditW final : public IButton {
private:
    // 左上角位置
    Vector2 position;
    Vector2 size;
    // 意为半径占高度的比例
    float radius = 0.0f;
    Vector3 bkColor = Vector3::zero;
    Vector3 fontColor = Vector3::one;
    Vector3 selColor = Vector3(0.0f, 0.0f, 1.0f);

    wchar_t text[DEFAULT_STRING_LENGTH + 1];
    bool editing = false;
    size_t editPos;

    void(*onEdit)(wchar_t*, void*) = NULL;
    void* userData = NULL;

public:
    UIEditW(Vector2 pos, float width);
    UIEditW(Vector2 pos, float width, void(*onEdit)(wchar_t*, void*));
    UIEditW(Vector2 pos, float width, void(*onEdit)(wchar_t*, void*), void* userData);
    virtual ~UIEditW();

    virtual bool Trigger(Vector2 pos) override;
    virtual void Hover(Vector2 pos) override;
    virtual void Click(Vector2 pos) override;
    virtual void Leave(Vector2 pos) override;
    virtual bool Unichar(wchar_t c) override;
    virtual void Render() override;

    wchar_t* GetText();
    size_t GetTextMaxLength();

    void SetBackgroundColor(Vector3 color);
    void SetFontColor(Vector3 color);
    void SetSelectionColor(Vector3 color);
    void SetCornerRadius(float radius);
    Vector3 GetBackgroundColor();
    Vector3 GetFontColor();
    Vector3 GetSelectionColor();
    float GetCornerRadius();
};

interface IOperation : public Object {
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

interface ITool : public Object {
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

interface IWindow : public Object {
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