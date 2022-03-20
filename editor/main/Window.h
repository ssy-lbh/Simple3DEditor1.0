#ifndef __EDITOR_MAIN_WINDOW__
#define __EDITOR_MAIN_WINDOW__

#include <define.h>

#include <utils/math3d/LinearAlgebra.h>

class AWindow : public Object {
protected:
    bool focus = false;

    Vector2 cursorCoord;
    Vector2 cursorPos = Vector2::zero;
    Vector2 cliSize;
    Vector2 cliInvSize;
    float aspect;

    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    AWindow();
    virtual ~AWindow();

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