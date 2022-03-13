#ifndef __EDITOR_RENDERWINDOW__
#define __EDITOR_RENDERWINDOW__

#include <define.h>

#include <editor/gui/UIManager.h>
#include <editor/gui/Menu.h>
#include <utils/gl/GLUtils.h>

class RenderWindow final : public IWindow {
private:
    bool focus = false;

    Vector2 cursorPos;
    Vector2 cliSize;
    GLRect rect;

    bool lightEnabled = false;

    Vector3 camLookat = Vector3::up;
    Quaternion camDir = Quaternion::one;
    float camDis = 5.0f;
    Vector3 camPos = Vector3::back * 5.0f + Vector3::up;
    Vector3 camRight = Vector3::right;
    Vector3 camUp = Vector3::up;
    Vector3 camForward = Vector3::forward;

    Menu* basicMenu;

    UIManager* uiMgr;

    GLProgram* glProg = NULL;

public:
    RenderWindow();
    virtual ~RenderWindow() override;

    void InitCamera();

    void UpdateWindowSize(int x, int y);
    void UpdateCursor(int x, int y);

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnCreate() override;
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

    void OnInsSave();

    void SaveImage(String file);
};

#endif