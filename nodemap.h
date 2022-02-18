#ifndef __NODEMAP__
#define __NODEMAP__

#include "uimgr.h"
#include "menu.h"
#include "gltools.h"

class NodeMapWindow : public IWindow {
private:
    bool focus = false;

    Vector2 cliSize;
    float aspect;
    Vector2 cursorPos;
    Vector2 viewPos;

    UIManager* uiMgr;
    UIManager* nodeMgr;

    Menu* basicMenu;

    GLTexture2D* bktex;

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
        virtual void Click(Vector2 pos) override;
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
        Node(Vector2 pos, NodeMapWindow* window);
        virtual ~Node() override;
        virtual bool Trigger(Vector2 pos) override;
        virtual void Render() override;
        virtual void Click(Vector2 pos) override;
        virtual void Drag(Vector2 dir) override;
        void Connect(Node* node);
        void Connect(Node* node, Vector2 offset);
        void Disconnect();
        void Disconnect(Node* node);
    };

    List<Node*> selectedNodes;
    
public:
    NodeMapWindow();
    virtual ~NodeMapWindow() override;

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnClose() override;
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

    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);
    void AddNode();
};

#endif