#include <editor/NodeMapWindow.h>

#include <lib/opengl/gl/gl.h>

#include <main.h>
#include <res.h>
#include <utils/gl/GLUtils.h>
#include <utils/gl/GLTexture2D.h>
#include <utils/os/Log.h>

NodeMapWindow::MoveButton::MoveButton(Vector2 center, float radius, NodeMapWindow* window) : center(center), radius(radius), window(window) {}

NodeMapWindow::MoveButton::~MoveButton(){}

bool NodeMapWindow::MoveButton::Trigger(Vector2 pos){
    return (pos - center).SqrMagnitude() <= radius * radius;
}

void NodeMapWindow::MoveButton::Render(){
    glColor3f(0.3f, 0.3f, 0.3f);
    GLUtils::DrawCorner(center.x, center.y, 0.0f, 360.0f, radius, 0.05f);
}

void NodeMapWindow::MoveButton::Click(Vector2 pos){
    start = window->viewPos;
}

void NodeMapWindow::MoveButton::Drag(Vector2 dir){
    window->viewPos = start - dir;
}

NodeMapWindow::Node::Node(NodeMapWindow* window) : Node(Vector2::zero, window) {}

NodeMapWindow::Node::Node(Vector2 pos, NodeMapWindow* window) : position(pos), window(window) {
    uiMgr = new UIManager();

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

NodeMapWindow::Node::~Node(){
    if (uiMgr) delete uiMgr;
}

bool NodeMapWindow::Node::Trigger(Vector2 pos){
    relaPos = pos - position;
    return relaPos.x >= 0.0f && relaPos.x <= 0.3f && relaPos.y >= 0.0f && relaPos.y <= 0.3f;
}

void NodeMapWindow::Node::Render(){
    focus ? glColor3f(0.5f, 0.5f, 0.1f) :
        window->selectedNodes.HasValue(this) ? glColor3f(0.3f, 0.3f, 0.1f) : glColor3f(0.05f, 0.05f, 0.05f);
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

    uiMgr->Render();
}

void NodeMapWindow::Node::Hover(Vector2 pos){
    uiMgr->CursorMove(pos - position);
}

void NodeMapWindow::Node::Click(Vector2 pos){
    start = position;
    uiMgr->LeftDown();
}

void NodeMapWindow::Node::Drag(Vector2 dir){
    position = start + dir;
}

void NodeMapWindow::Node::ClickEnd(Vector2 pos, IButton* end){
    uiMgr->LeftUp();
}

void NodeMapWindow::Node::OnFocus(Vector2 pos){
    if (!window->selectedNodes.HasValue(this))
        window->selectedNodes.Add(this);
    focus = true;
}

void NodeMapWindow::Node::OnKillFocus(Vector2 pos, IButton* focus){
    this->focus = false;
}

void NodeMapWindow::Node::RightClick(Vector2 pos){
    rightStart = pos;
    rightEnd = pos;
    rightDown = true;
}

void NodeMapWindow::Node::RightDrag(Vector2 dir){
    rightEnd = rightStart + dir;
}

void NodeMapWindow::Node::RightClickEnd(Vector2 pos, IButton* end){
    Connect((Node*)end);
    rightDown = false;
}

bool NodeMapWindow::Node::Char(char c){
    return uiMgr->Char(c);
}

void NodeMapWindow::Node::Connect(Node* node){
    Connect(node, Vector2(0.0f, 0.15f));
}

void NodeMapWindow::Node::Connect(Node* node, Vector2 offset){
    if (node == this)
        return;
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
    DebugLog("NodeMapWindow Launched");
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
    DebugLog("NodeMapWindow Destroyed");
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
    uiMgr->Render();

    glPushMatrix();
    glTranslatef(-viewPos.x, -viewPos.y, 0.0f);
    nodeMgr->Render();
    glPopMatrix();
}

void NodeMapWindow::OnCreate(){
    bktex = new GLTexture2D(IDT_NODEMAP_BACKGROUND);
}

void NodeMapWindow::OnChar(char c){
    //DebugLog("NodeMapWindow::OnChar");
    uiMgr->Char(c);
    nodeMgr->Char(c);
}

void NodeMapWindow::OnUnichar(wchar_t c){
    //DebugLog("NodeMapWindow::OnUnichar");
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
    nodeMgr->CursorMove(cursorPos + viewPos);
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
    UpdateCursor(x, y);
    if (nodeMgr->RightDown())
        return;
    Main::SetMenu(basicMenu);
}

void NodeMapWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
    nodeMgr->RightUp();
}

void NodeMapWindow::OnMouseHover(int key, int x, int y){}

void NodeMapWindow::OnMouseLeave(){}

void NodeMapWindow::OnFocus(){
    focus = true;
}

void NodeMapWindow::OnKillFocus(){
    focus = false;
}

void NodeMapWindow::OnMouseWheel(int delta){}

void NodeMapWindow::OnMenuAccel(int id, bool accel){
    switch (id){
    case IDM_MENU_BASIC:
        Main::SetMenu(basicMenu);
        break;
    }
}