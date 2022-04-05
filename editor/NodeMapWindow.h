#ifndef __EDITOR_NODEMAPWINDOW__
#define __EDITOR_NODEMAPWINDOW__

#include <define.h>

#include <utils/math3d/LinearAlgebra.h>
#include <editor/main/Window.h>
#include <editor/object/GUIObject.h>

class NodeMapWindow final : public AWindow {
private:
    Vector2 viewPos;

    GUIManagerObject* guiMgr;
    GUIManagerObject* nodeMgr;

    Menu* basicMenu;

    GLTexture2D* bktex = NULL;

    class MoveButton final : public AGUIObject {
    private:
        Point2 center;
        float radius;
        Point2 start;
        NodeMapWindow* window;

    public:
        MoveButton(Point2 center, float radius, NodeMapWindow* window);
        virtual ~MoveButton() override;

        virtual bool OnHit2D(Point2 pos) override;
        virtual void OnRender() override;
        virtual void OnLeftDown2D(Point2 pos) override;
        virtual void OnLeftDrag2D(Vector2 dir) override;
    };

    class Node final : public AGUIObject {
    private:
        NodeMapWindow* window;
        Point2 relaPos;
        Point2 start;
        Point2 rightStart;
        Point2 rightEnd;
        Point2 position;
        Node* connNode = NULL;
        Vector2 offset;
        bool focus = false;
        bool rightDown = false;

        GUIManagerObject* guiMgr = NULL;

    public:
        Node(NodeMapWindow* window);
        Node(Point2 pos, NodeMapWindow* window);
        virtual ~Node() override;

        virtual bool OnHit2D(Point2 pos) override;
        virtual void OnRender() override;
        virtual void OnMouseMove2D(Point2 pos) override;
        virtual void OnLeftDown2D(Point2 pos) override;
        virtual void OnLeftDrag2D(Vector2 dir) override;
        virtual void OnLeftUp2D(Point2 pos) override;
        virtual void OnFocus() override;
        virtual void OnKillFocus() override;
        virtual void OnRightDown2D(Point2 pos) override;
        virtual void OnRightDrag2D(Vector2 dir) override;
        virtual void OnRightUp2D(Point2 pos) override;
        virtual void OnChar(char c) override;

        void Connect(Node* node);
        void Connect(Node* node, Vector2 offset);
        void Disconnect();
        void Disconnect(Node* node);
    };

    List<Node*> selectedNodes;

protected:
    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    NodeMapWindow();
    virtual ~NodeMapWindow() override;

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
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;

    void AddNode();
};

#endif