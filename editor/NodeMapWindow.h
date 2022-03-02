#ifndef __EDITOR_NODEMAPWINDOW__
#define __EDITOR_NODEMAPWINDOW__

#include <define.h>

#include <editor/gui/UIManager.h>
#include <editor/gui/Menu.h>
#include <utils/gl/GLUtils.h>

class NodeMapWindow final : public IWindow {
private:
    bool focus = false;

    Vector2 cliSize;
    float aspect;
    Vector2 cursorPos;
    Vector2 viewPos;

    UIManager* uiMgr;
    UIManager* nodeMgr;

    Menu* basicMenu;

    GLTexture2D* bktex = NULL;

    class MoveButton final : public IButton {
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

    class Node final : public IButton {
    private:
        NodeMapWindow* window;
        Vector2 relaPos;
        Vector2 start;
        Vector2 rightStart;
        Vector2 rightEnd;
        Vector2 position;
        Node* connNode = NULL;
        Vector2 offset;
        bool focus = false;
        bool rightDown = false;

        UIManager* uiMgr = NULL;

    public:
        Node(NodeMapWindow* window);
        Node(Vector2 pos, NodeMapWindow* window);
        virtual ~Node() override;
        virtual bool Trigger(Vector2 pos) override;
        virtual void Render() override;
        virtual void Hover(Vector2 pos) override;
        virtual void Click(Vector2 pos) override;
        virtual void Drag(Vector2 dir) override;
        virtual void ClickEnd(Vector2 pos, IButton* end) override;
        virtual void OnFocus(Vector2 pos);
        virtual void OnKillFocus(Vector2 pos, IButton* focus);
        virtual void RightClick(Vector2 pos);
        virtual void RightDrag(Vector2 dir);
        virtual void RightClickEnd(Vector2 pos, IButton* end);
        virtual bool Char(char c);

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
    virtual void OnCreate() override;
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