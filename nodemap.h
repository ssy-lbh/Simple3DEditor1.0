#ifndef __NODEMAP__
#define __NODEMAP__

#include "uimgr.h"

class NodeMapWindow;

class NodeMapWindow : IWindow {
private:
    bool focus;
    
public:
    NodeMapWindow();
    virtual ~NodeMapWindow() override;

    virtual void SetFrame(HWND hWnd) override;
    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnCreate() override;
    virtual void OnClose() override;
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
    virtual void OnControl(int inform, int id, HWND hctl) override;
};

#endif