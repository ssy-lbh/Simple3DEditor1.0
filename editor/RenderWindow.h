#ifndef __EDITOR_RENDERWINDOW__
#define __EDITOR_RENDERWINDOW__

#include <define.h>

#include <utils/math3d/Camera.h>
#include <editor/main/Window.h>

class RenderWindow final : public AWindow, public CCamera {
private:
    Rect rect;

    bool lightEnabled = false;

    Menu* basicMenu;

    GUIManagerObject* guiMgr;

protected:
    void UpdateWindowSize(int x, int y);
    void UpdateCursor(int x, int y);

    void SetLookAt(Point3 at);
    void SetRotation(Quaternion rot);
    void SetDistance(float dis);
    void InitCamera();

public:
    RenderWindow();
    virtual ~RenderWindow() override;
    
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
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path) override;
    virtual void OnDropFileW(const wchar_t* path) override;

    void OnInsSave();

    void SaveImage(String file);
};

#endif