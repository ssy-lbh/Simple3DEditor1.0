#ifndef __NODEMAP__
#define __NODEMAP__

#include "uimgr.h"

class NodeMapWindow;

class NodeMapWindow : public IWindow {
private:
    HWND hWnd;
    bool focus;

    Vector2 cliSize;
    Vector2 cursorPos;
    Vector2 viewPos;

    UIManager* uiMgr;
    UIManager* nodeMgr;

    class MoveButton : public IButton {
    private:
        Vector2 center;
        float radius;
        Vector2 start;
        NodeMapWindow* window;
    public:
        MoveButton(Vector2 center, float radius, NodeMapWindow* window);
        virtual ~MoveButton() override;
        virtual bool Trigger(Vector2 pos) override;
        virtual void Render() override;
        virtual void Click() override;
        virtual void Drag(Vector2 dir) override;
    };

    class Node : public IButton {
    private:
        NodeMapWindow* window;
        Vector2 start;
        Vector2 position;
        Node* connNode = NULL;
        Vector2 offset;
    public:
        Node(NodeMapWindow* window);
        virtual ~Node() override;
        virtual bool Trigger(Vector2 pos) override;
        virtual void Render() override;
        virtual void Click() override;
        virtual void Drag(Vector2 dir) override;
    };
    
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

    void UpdateCursor(int x, int y);
};

#endif