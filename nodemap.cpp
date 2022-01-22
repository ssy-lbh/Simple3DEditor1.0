#include "nodemap.h"

#include <gl/gl.h>

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

NodeMapWindow::Node::~Node(){}

bool NodeMapWindow::Node::Trigger(Vector2 pos){
    Vector2 rela = pos - position;
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
    glColor3f(1.0f, 1.0f, 0.2f);
    Vector2 begin = Vector2(position.x + 0.3f, position.y + 0.15f);
    GLUtils::DrawBezier(begin, begin + Vector2(0.5f, 0.0f), Vector2(0.5f, 0.0f), Vector2(1.0f, 0.0f), 0.01f);
}

void NodeMapWindow::Node::Click(){
    start = position;
}

void NodeMapWindow::Node::Drag(Vector2 dir){
    position = start + dir;
}

NodeMapWindow::NodeMapWindow(){
    uiMgr = new UIManager();
    nodeMgr = new UIManager();

    uiMgr->AddButton(new MoveButton(Vector2(0.85f, 0.85f), 0.12f, this));

    nodeMgr->AddButton(new Node(this));
}

NodeMapWindow::~NodeMapWindow(){
    if(uiMgr) delete uiMgr;
    if(nodeMgr) delete nodeMgr;
}

void NodeMapWindow::SetFrame(HWND hWnd){
    this->hWnd = hWnd;
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

    // UI绘制
    uiMgr->Render();
    nodeMgr->Render();

    glDisable(GL_LINE_STIPPLE);
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    GLUtils::DrawBezier(Vector2(-1.0f, -1.0f), Vector2(0.0f, -1.0f), Vector2(0.0f, 1.0f), Vector2(1.0f, 1.0f), 0.01f);
}

void NodeMapWindow::OnCreate(){}

void NodeMapWindow::OnClose(){}

void NodeMapWindow::OnResize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
}

void NodeMapWindow::UpdateCursor(int x, int y){
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 2.0f * y / cliSize.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    nodeMgr->CursorMove(cursorPos);
}

void NodeMapWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void NodeMapWindow::OnLeftDown(int x, int y){
    uiMgr->LeftDown();
    nodeMgr->LeftDown();
    UpdateCursor(x, y);
}

void NodeMapWindow::OnLeftUp(int x, int y){
    uiMgr->LeftUp();
    nodeMgr->LeftUp();
    UpdateCursor(x, y);
}

void NodeMapWindow::OnRightDown(int x, int y){}

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

void NodeMapWindow::OnControl(int inform, int id, HWND hctl){}