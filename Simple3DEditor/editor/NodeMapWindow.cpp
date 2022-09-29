#include <editor/NodeMapWindow.h>

#include <main.h>
#include <res.h>
#include <util/gl/GLUtils.h>
#include <util/gl/GLTexture2D.h>
#include <util/gl/GLSimplified.h>
#include <util/os/Log.h>
#include <editor/gui/Menu.h>
#include <editor/object/GUIManagerObject.h>

#include <lib/json/nlohmann/json.hpp>

namespace simple3deditor {

class NodeMapWindow::MoveButton : public AGUIObject {
private:
    Point2 center;
    float radius;
    Point2 start;
    NodeMapWindow* window;

public:
    MoveButton(Point2 center, float radius, NodeMapWindow* window) : center(center), radius(radius), window(window) {}
    virtual ~MoveButton() override{}

    virtual bool OnHit2D(Point2 pos) override{
        return (pos - center).SqrMagnitude() <= radius * radius;
    }

    virtual void OnRender() override{
        glColor3f(0.3f, 0.3f, 0.3f);
        GLUtils::DrawCorner(center.x, center.y, 0.0f, 360.0f, radius, 0.05f);
    }

    virtual void OnLeftDown2D(Point2 pos) override{
        start = window->viewPos;
    }

    virtual void OnLeftDrag2D(Vector2 dir) override{
        window->viewPos = start - dir;
    }
};

class NodeMapWindow::Node : public AGUIObject {
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
    Node(NodeMapWindow* window) : Node(Point2::zero, window) {}

    Node(Point2 pos, NodeMapWindow* window) : position(pos), window(window) {
        guiMgr = new GUIManagerObject();

        //TODO 做好适配变换的组件
        // UIEditA* name = new UIEditA(Vector2(0.05f, 0.0f), 0.2f, [](char* s, void* user){
        //     DebugLog("Edit %s", s);
        // });
        // name->SetCornerRadius(0.2f);
        // name->SetBackgroundColor(Vector3(0.1f, 0.1f, 0.1f));
        // name->SetFontColor(Vector3(1.0f, 1.0f, 1.0f));
        // name->SetSelectionColor(Vector3(0.0f, 0.0f, 1.0f));
        // uiMgr->AddButton(name);
    }

    virtual ~Node() override{
        if (guiMgr) delete guiMgr;
    }

    virtual bool OnHit2D(Point2 pos) override{
        relaPos = pos - position;
        return relaPos.x >= 0.0f && relaPos.x <= 0.3f && relaPos.y >= 0.0f && relaPos.y <= 0.3f;
    }

    virtual void OnRender() override{
        glColorv3(focus ? Vector3(0.5f, 0.5f, 0.1f) :
            window->selectedNodes.HasValue(this) ? Vector3(0.3f, 0.3f, 0.1f) : Vector3(0.05f, 0.05f, 0.05f));
        GLUtils::DrawRoundRect(
            position.x,
            position.y,
            0.3f,
            0.3f,
            0.05f,
            0.05f
        );

        if (connNode){
            glColor3f(1.0f, 1.0f, 0.2f);
            glLineWidth(1.0f);
            Vector2 begin = Vector2(position.x + 0.3f, position.y + 0.15f);
            //DebugLog("access %p", connNode);
            Vector2 end = connNode->position + offset;
            GLUtils::DrawBezier(begin, begin + Vector2(0.3f, 0.0f), end - Vector2(0.3f, 0.0f), end, 0.01f);
        }

        if (rightDown){
            glColor3f(0.5f, 0.5f, 0.2f);
            glLineWidth(1.0f);
            GLUtils::DrawBezier(rightStart, rightStart + Vector2(0.3f, 0.0f), rightEnd - Vector2(0.3f, 0.0f), rightEnd, 0.01f);
        }

        guiMgr->OnChainRender();
    }

    virtual void OnMouseMove2D(Point2 pos) override{
        guiMgr->OnMouseMove2D(pos - position);
    }

    virtual void OnLeftDown2D(Point2 pos) override{
        start = position;
        guiMgr->OnLeftDown2D(pos - position);
    }

    virtual void OnLeftDrag2D(Vector2 dir) override{
        position = start + dir;
    }

    virtual void OnLeftUp2D(Point2 pos) override{
        guiMgr->OnLeftUp2D(pos - position);
    }

    virtual void OnFocus() override{
        if (!window->selectedNodes.HasValue(this))
            window->selectedNodes.Add(this);
        focus = true;
    }

    virtual void OnKillFocus() override{
        this->focus = false;
    }

    virtual void OnRightDown2D(Point2 pos) override{
        rightStart = pos;
        rightEnd = pos;
        rightDown = true;
    }

    virtual void OnRightDrag2D(Vector2 dir) override{
        rightEnd = rightStart + dir;
    }

    virtual void OnRightUp2D(Point2 pos) override{
        if (AGUIObject::parentGUIMgr){
            try {
                Connect(dynamic_cast<Node*>(AGUIObject::parentGUIMgr->GetCurrent()));
            }catch(std::bad_cast e){
                DebugError("Connect To A Non-node Object");
            }
        }
        rightDown = false;
    }

    virtual void OnChar(char c) override{
        guiMgr->OnChar(c);
    }

    void Connect(Node* node){
        Connect(node, Vector2(0.0f, 0.15f));
    }

    void Connect(Node* node, Vector2 offset){
        if (node == this)
            return;
        connNode = node;
        this->offset = offset;
    }

    void Disconnect(){
        connNode = NULL;
    }
    
    void Disconnect(Node* node){
        DebugLog("NodeMapWindow::Node::Disconnect %p %p", this, node);
        if (connNode == node)
            connNode = NULL;
    }
};

WINDOW_INFO_DECL(simple3deditor::NodeMapWindow, L"节点图编辑器");

NodeMapWindow::NodeMapWindow(){
    DebugLog("NodeMapWindow Launched");
    guiMgr = new GUIManagerObject();
    nodeMgr = new GUIManagerObject();

    guiMgr->AddChild(new MoveButton(Vector2(0.85f, 0.85f), 0.12f, this));

    Node* n1 = new Node(Vector2(-0.5f, 0.5f), this);
    Node* n2 = new Node(Vector2(-0.5f, -0.5f), this);
    nodeMgr->AddChild(n1);
    nodeMgr->AddChild(n2);
    n1->Connect(n2);

    basicMenu = new Menu();
    basicMenu->AddItem(new MenuItem(L"添加节点", [=]{ this->AddNode(); }));
    basicMenu->AddItem(new MenuItem(L"删除节点", [=]{
        if (this->selectedNodes.Size() == 1){
            Node* node = this->selectedNodes[0];
            this->nodeMgr->DeleteChild(node);
            this->nodeMgr->GetChildren().Foreach([=](AViewObject* btn){
                if (btn){
                    ((Node*)btn)->Disconnect(node);
                }else{
                    DebugError("NodeMapWindow::nodeMgr has NULL button");
                }
            });
            this->selectedNodes.Clear();
            delete node;
        }
    }));
    basicMenu->AddItem(new MenuItem(L"链接节点", [=]{
        if (this->selectedNodes.Size() == 2){
            Node* n1 = this->selectedNodes[0];
            Node* n2 = this->selectedNodes[1];
            n1->Connect(n2);
            this->selectedNodes.Clear();
        }
    }));
    basicMenu->AddItem(new MenuItem(L"断开节点", [=]{
        if (this->selectedNodes.Size() == 1){
            Node* node = this->selectedNodes[0];
            node->Disconnect();
            this->selectedNodes.Clear();
        }
    }));

    bktex = new GLTexture2D(IDT_NODEMAP_BACKGROUND);
}

NodeMapWindow::~NodeMapWindow(){
    DebugLog("NodeMapWindow Destroyed");
    if (guiMgr) delete guiMgr;
    if (nodeMgr) delete nodeMgr;
    if (basicMenu) delete basicMenu;
    if (bktex) delete bktex;
}

void NodeMapWindow::OnRender(){
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_AUTO_NORMAL);
    glEnable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    bktex->Enable();
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();
    bktex->Disable();

    // UI绘制
    guiMgr->OnChainRender();

    nodeMgr->transform.position = -viewPos;
    nodeMgr->OnChainRender();
}

void NodeMapWindow::OnChar(char c){
    guiMgr->OnChar(c);
    nodeMgr->OnChar(c);
}

void NodeMapWindow::OnUnichar(wchar_t c){
    guiMgr->OnUnichar(c);
    nodeMgr->OnUnichar(c);
}

void NodeMapWindow::OnResize(int x, int y){
    AWindow::OnResize(x, y);
    UpdateWindowSize(x, y);
}

void NodeMapWindow::UpdateCursor(int x, int y){
    AWindow::UpdateCursor(x, y);
    guiMgr->OnMouseMove2D(cursorPos);
    nodeMgr->OnMouseMove2D(cursorPos + viewPos);
}

void NodeMapWindow::UpdateWindowSize(int x, int y){}

void NodeMapWindow::AddNode(){
    Node* node = new Node(cursorPos, this);
    nodeMgr->AddChild(node);
}

void NodeMapWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void NodeMapWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    // UI交互
    guiMgr->OnLeftDown2D(cursorPos);
    if (guiMgr->GetCurrent())
        return;
    // 节点交互
    nodeMgr->OnLeftDown2D(cursorPos);
    if (nodeMgr->GetCurrent()){
        return;
    }else{
        selectedNodes.Clear();
    }
}

void NodeMapWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnLeftUp2D(cursorPos);
    nodeMgr->OnLeftUp2D(cursorPos);
}

void NodeMapWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
    nodeMgr->OnRightDown2D(cursorPos);
    if (nodeMgr->GetCurrent())
        return;
    Main::SetMenu(basicMenu);
}

void NodeMapWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
    nodeMgr->OnRightUp2D(cursorPos);
}

void NodeMapWindow::OnMouseWheel(int delta){}

void NodeMapWindow::OnMenuAccel(int id, bool accel){
    switch (id){
    case IDM_MENU_BASIC:
        Main::SetMenu(basicMenu);
        break;
    }
}

void NodeMapWindow::Serialize(nlohmann::json& o){
    o["id"] = WINDOW_ID;
}

void NodeMapWindow::Deserialize(nlohmann::json& o){}

}