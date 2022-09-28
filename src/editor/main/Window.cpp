#include <editor/main/Window.h>

#include <editor/object/GUIManagerObject.h>

#include <lib/json/nlohmann/json.hpp>

namespace simple3deditor {

WINDOW_INFO_DECL(simple3deditor::AWindow, L"");

AWindow::AWindow(){}
AWindow::~AWindow(){}

void AWindow::UpdateCursor(int x, int y){
    cursorCoord.x = x;
    cursorCoord.y = y;
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 2.0f * y / cliSize.y - 1.0f;
}

void AWindow::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
    cliInvSize.x = 1.0f / cliSize.x;
    cliInvSize.y = 1.0f / cliSize.y;
    aspect = (float)cliSize.x / cliSize.y;
}

bool AWindow::IsFocus(){
    return focus;
}

void AWindow::OnForwardRender(){}
void AWindow::OnRender(){}
void AWindow::OnPostRender(){}
void AWindow::OnTimer(int id){}
void AWindow::OnChar(char c){}
void AWindow::OnUnichar(wchar_t c){}

void AWindow::OnResize(int x, int y){
    if (x == 0 || y == 0){
        return;
    }
    UpdateWindowSize(x, y);
}

void AWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void AWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
}

void AWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
}

void AWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
}

void AWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
}

void AWindow::OnFocus(){
    focus = true;
}

void AWindow::OnKillFocus(){
    focus = false;
}

void AWindow::OnMouseWheel(int delta){}
void AWindow::OnMenuAccel(int id, bool accel){}
void AWindow::OnDropFileA(const char* path, uint len){}
void AWindow::OnDropFileW(const wchar_t* path, uint len){}

void AWindow::Serialize(nlohmann::json& o){
    o["id"] = WINDOW_ID;
}

void AWindow::Deserialize(nlohmann::json& o){}

const char* AGUIWindow::WINDOW_ID = "lbh.base.guiwnd";
const wchar_t* AGUIWindow::WINDOW_DISPLAY_NAME = L"";

AGUIWindow::AGUIWindow(){
    guiMgr = new GUIManagerObject();
}

AGUIWindow::~AGUIWindow(){
    delete guiMgr;
}

void AGUIWindow::OnRender(){
    guiMgr->OnChainRender();   
}

void AGUIWindow::OnChar(char c){
    guiMgr->OnChar(c);
}

void AGUIWindow::OnUnichar(wchar_t c){
    guiMgr->OnUnichar(c);
}

void AGUIWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnMouseMove2D(cursorPos);
}

void AGUIWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnLeftDown2D(cursorPos);
}

void AGUIWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnLeftUp2D(cursorPos);
}

void AGUIWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnRightDown2D(cursorPos);
}

void AGUIWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnRightUp2D(cursorPos);
}

void AGUIWindow::Serialize(nlohmann::json& o){
    o["id"] = WINDOW_ID;
}

}