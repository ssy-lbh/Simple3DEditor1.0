#include <editor/AttributeWindow.h>

#include <lib/glew/gl/glew.h>

#include <util/gl/GLUtils.h>
#include <editor/gui/GUIUtils.h>
#include <editor/object/GUIManagerObject.h>

#include <lib/json/nlohmann/json.hpp>

namespace simple3deditor {

WINDOW_INFO_DECL(simple3deditor::AttributeWindow, L"属性窗口(未完成)");

AttributeWindow::AttributeWindow(){
    DebugLog("AttributeWindow Launched");
    guiMgr = new GUIManagerObject();

    // 注意：字体，编辑框高度为30px
    GUIEditA* edit = new GUIEditA(Vector2(0.0f, 0.0f), 0.5f);
    guiMgr->AddChild(edit);
}

AttributeWindow::~AttributeWindow(){
    DebugLog("AttributeWindow Destroyed");
    if (guiMgr) delete guiMgr;
}

void AttributeWindow::OnRender(){
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
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    GLUtils::ResetProjection();
    GLUtils::ResetModelView();

    // 缩放至按像素为单位的坐标系，左上角为原点
    guiMgr->transform.scale.Set(Vector3(cliSize.x * 0.5f, -cliSize.y * 0.5f, 1.0f));
    guiMgr->transform.position.Set(Vector3(-cliSize.x * 0.5f, cliSize.y * 0.5f, 0.0f));
    guiMgr->OnChainRender();
}

void AttributeWindow::OnResize(int x, int y){
    AWindow::OnResize(x, y);
    UpdateWindowSize(x, y);
}

void AttributeWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnMouseMove2D(cursorPos);
}

void AttributeWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnLeftDown2D(cursorPos);
}

void AttributeWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnLeftUp2D(cursorPos);
}

void AttributeWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnRightDown2D(cursorPos);
}

void AttributeWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnRightUp2D(cursorPos);
}

void AttributeWindow::OnMouseWheel(int delta){}

void AttributeWindow::OnMenuAccel(int id, bool accel){}

void AttributeWindow::Serialize(nlohmann::json& o){
    o["id"] = WINDOW_ID;
}

void AttributeWindow::Deserialize(nlohmann::json& o){}

void AttributeWindow::UpdateWindowSize(int x, int y){}

void AttributeWindow::UpdateCursor(int x, int y){
    AWindow::UpdateCursor(x, y);
}

}