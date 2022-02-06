#include "nodemap.h"

#include "opengl/gl/gl.h"

#include "main.h"
#include "res.h"
#include "gltools.h"
#include "log.h"

NodeMapWindow::MoveButton::MoveButton(Vector2 center, float radius, NodeMapWindow* window) : center(center), radius(radius), window(window) {}

NodeMapWindow::MoveButton::~MoveButton(){}

bool NodeMapWindow::MoveButton::Trigger(Vector2 pos){
    return (pos - center).SqrMagnitude() <= radius * radius;
}

void NodeMapWindow::MoveButton::Render(){
    glColor3f(0.3f, 0.3f, 0.3f);
    GLUtils::DrawCorner(center.x, center.y, 0.0f, 360.0f, radius, 0.05f);
}

void NodeMapWindow::MoveButton::Click(){
    start = window->viewPos;
}

void NodeMapWindow::MoveButton::Drag(Vector2 dir){
    window->viewPos = start - dir;
}

NodeMapWindow::Node::Node(NodeMapWindow* window) : window(window) {}

NodeMapWindow::Node::Node(Vector2 pos, NodeMapWindow* window) : position(pos), window(window) {}

NodeMapWindow::Node::~Node(){}

bool NodeMapWindow::Node::Trigger(Vector2 pos){
    Vector2 rela = pos - position + window->viewPos;
    return rela.x >= 0.0f && rela.x <= 0.3f && rela.y >= 0.0f && rela.y <= 0.3f;
}

void NodeMapWindow::Node::Render(){
    glColor3f(0.05f, 0.05f, 0.05f);
    GLUtils::DrawRoundRect(
        position.x,
        position.y,
        0.3f,
        0.3f,
        0.05f,
        0.05f
    );
    if (connNode){
        glDisable(GL_LINE_STIPPLE);
        glColor3f(1.0f, 1.0f, 0.2f);
        glLineWidth(1.0f);
        Vector2 begin = Vector2(position.x + 0.3f, position.y + 0.15f);
        //DebugLog("access %p", connNode);
        Vector2 end = connNode->position + offset;
        GLUtils::DrawBezier(begin, begin + Vector2(0.3f, 0.0f), end - Vector2(0.3f, 0.0f), end, 0.01f);
    }
}

void NodeMapWindow::Node::Click(){
    start = position;
    if (!window->selectedNodes.HasValue(this))
        window->selectedNodes.Add(this);
}

void NodeMapWindow::Node::Drag(Vector2 dir){
    position = start + dir;
}

void NodeMapWindow::Node::Connect(Node* node){
    connNode = node;
    offset = Vector2(0.0f, 0.15f);
}

void NodeMapWindow::Node::Connect(Node* node, Vector2 offset){
    connNode = node;
    this->offset = offset;
}

void NodeMapWindow::Node::Disconnect(){
    connNode = NULL;
}

void NodeMapWindow::Node::Disconnect(Node* node){
    DebugLog("NodeMapWindow::Node::Disconnect %p %p", this, node);
    if (connNode == node){
        connNode = NULL;
    }
}

NodeMapWindow::NodeMapWindow(){
    uiMgr = new UIManager();
    nodeMgr = new UIManager();

    uiMgr->AddButton(new MoveButton(Vector2(0.85f, 0.85f), 0.12f, this));

    Node* n1 = new Node(Vector2(-0.5f, 0.5f), this);
    Node* n2 = new Node(Vector2(-0.5f, -0.5f), this);
    nodeMgr->AddButton(n1);
    nodeMgr->AddButton(n2);
    n1->Connect(n2);

    basicMenu = new Menu();
    basicMenu->AddItem(new MenuItem(L"添加节点", MENUITEM_LAMBDA_TRANS(NodeMapWindow)[](NodeMapWindow* window){
        window->AddNode();
    }, this));
    basicMenu->AddItem(new MenuItem(L"删除节点", MENUITEM_LAMBDA_TRANS(NodeMapWindow)[](NodeMapWindow* window){
        if (window->selectedNodes.Size() == 1){
            Node* node = window->selectedNodes.GetItem(0);
            window->nodeMgr->DeleteButton(node);
            window->nodeMgr->Foreach([](IButton* btn, void* node){
                if (btn){
                    ((Node*)btn)->Disconnect((Node*)node);
                }else{
                    DebugError("NodeMapWindow::nodeMgr has NULL button");
                }
            }, node);
            window->selectedNodes.Clear();
            //DebugLog("delete %p", node);
            delete node;
        }
    }, this));
    basicMenu->AddItem(new MenuItem(L"链接节点", MENUITEM_LAMBDA_TRANS(NodeMapWindow)[](NodeMapWindow* window){
        if (window->selectedNodes.Size() == 2){
            Node* n1 = window->selectedNodes.GetItem(0);
            Node* n2 = window->selectedNodes.GetItem(1);
            n1->Connect(n2);
            window->selectedNodes.Clear();
        }
    }, this));
    basicMenu->AddItem(new MenuItem(L"断开节点", MENUITEM_LAMBDA_TRANS(NodeMapWindow)[](NodeMapWindow* window){
        if (window->selectedNodes.Size() == 1){
            Node* node = window->selectedNodes.GetItem(0);
            node->Disconnect();
            window->selectedNodes.Clear();
        }
    }, this));
}

NodeMapWindow::~NodeMapWindow(){
    if (uiMgr) delete uiMgr;
    if (nodeMgr) delete nodeMgr;
    if (basicMenu) delete basicMenu;
    if (bktex) delete bktex;
}

bool NodeMapWindow::IsFocus(){
    return focus;
}

void NodeMapWindow::OnRender(){
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    if (!bktex){
        bktex = new GLTexture2D(IDB_NODEMAP_BACKGROUND);
    }
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
    uiMgr->Render();

    nodeMgr->RenderTransform();

    glPushMatrix();
    glTranslatef(-viewPos.x, -viewPos.y, 0.0f);

    nodeMgr->RenderRaw();

    glPopMatrix();
}

void NodeMapWindow::OnClose(){}

void NodeMapWindow::OnChar(char c){
    uiMgr->Char(c);
    nodeMgr->Char(c);
}

void NodeMapWindow::OnUnichar(wchar_t c){
    uiMgr->Unichar(c);
    nodeMgr->Unichar(c);
}

void NodeMapWindow::OnResize(int x, int y){
    UpdateWindowSize(x, y);
}

void NodeMapWindow::UpdateCursor(int x, int y){
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 2.0f * y / cliSize.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    nodeMgr->CursorMove(cursorPos);
}

void NodeMapWindow::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
    aspect = (float)cliSize.x / cliSize.y;
}

void NodeMapWindow::AddNode(){
    Node* node = new Node(cursorPos, this);
    nodeMgr->AddButton(node);
}

void NodeMapWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void NodeMapWindow::OnLeftDown(int x, int y){
    // UI交互
    if (uiMgr->LeftDown()){
        UpdateCursor(x, y);
        return;
    }
    // 节点交互
    if (nodeMgr->LeftDown()){
        UpdateCursor(x, y);
        return;
    }else{
        selectedNodes.Clear();
    }
}

void NodeMapWindow::OnLeftUp(int x, int y){
    uiMgr->LeftUp();
    nodeMgr->LeftUp();
    UpdateCursor(x, y);
}

void NodeMapWindow::OnRightDown(int x, int y){
    Main::SetMenu(basicMenu);
}

void NodeMapWindow::OnRightUp(int x, int y){}

void NodeMapWindow::OnMouseHover(int key, int x, int y){}

void NodeMapWindow::OnMouseLeave(){}

void NodeMapWindow::OnFocus(){
    focus = true;
}

void NodeMapWindow::OnKillFocus(){
    focus = false;
}

void NodeMapWindow::OnMouseWheel(int delta){}

void NodeMapWindow::OnMenuAccel(int id, bool accel){}