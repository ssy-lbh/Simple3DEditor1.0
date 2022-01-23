#include "container.h"

#include "gltools.h"
#include "log.h"

LRContainer::LRContainer(IWindow* lWindow, IWindow* rWindow) : lWindow(lWindow), rWindow(rWindow) {}

LRContainer::~LRContainer(){}

bool LRContainer::IsFocus(){
    return focus != NULL;
}

void LRContainer::OnRender(){
    RECT tmp;

    tmp = ViewportManager::inst->GetCurrentRect();
    tmp.right = tmp.left + dis;
    ViewportManager::inst->PushViewport(tmp);
    lWindow->OnRender();
    ViewportManager::inst->PopViewport();

    tmp = ViewportManager::inst->GetCurrentRect();
    tmp.left = tmp.left + dis;
    ViewportManager::inst->PushViewport(tmp);
    rWindow->OnRender();
    ViewportManager::inst->PopViewport();
}

void LRContainer::OnCreate(HWND hWnd){
    lWindow->OnCreate(hWnd);
    rWindow->OnCreate(hWnd);
}

void LRContainer::OnClose(){
    lWindow->OnClose();
    rWindow->OnClose();
}

void LRContainer::OnResize(int x, int y){
    size.x = x;
    size.y = y;
    dis = GLUtils::Clamp(dis, 0.0f, size.x);
    lWindow->OnResize(dis, y);
    rWindow->OnResize(x - dis, y);
}

void LRContainer::OnMouseMove(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    if (adjustPos){
        SetCursor(LoadCursor(NULL, IDC_SIZEWE));
        dis = x;
        lWindow->OnResize(dis, size.y);
        rWindow->OnResize(size.x - dis, size.y);
        return;
    }
    if (focus)
        focus->OnMouseMove(right ? x - dis : x, y);
}

void LRContainer::OnLeftDown(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    if (Abs(x - dis) < 4.0f){
        adjustPos = true;
        SetCursor(LoadCursor(NULL, IDC_SIZEWE));
        return;
    }
    UpdateFocus();
    focus->OnLeftDown(right ? x - dis : x, y);
}

void LRContainer::OnLeftUp(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    if (adjustPos){
        adjustPos = false;
    }
    if (focus)
        focus->OnLeftUp(right ? x - dis : x, y);
}

void LRContainer::OnRightDown(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    UpdateFocus();
    focus->OnRightDown(right ? x - dis : x, y);
}

void LRContainer::OnRightUp(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    if (focus)
        focus->OnRightUp(right ? x - dis : x, y);
}

void LRContainer::OnMouseHover(int key, int x, int y){
    if (focus)
        focus->OnMouseHover(key, right ? x - dis : x, y);
}

void LRContainer::OnMouseLeave(){
    if (focus)
        focus->OnMouseLeave();
}

void LRContainer::OnFocus(){
    UpdateFocus();
}

void LRContainer::OnKillFocus(){
    focus = NULL;
    DebugLog("LRContainer::focus %p", focus);
}

void LRContainer::OnMouseWheel(int delta){
    if (focus)
        focus->OnMouseWheel(delta);
}

void LRContainer::OnMenuAccel(int id, bool accel){
    if (focus)
        focus->OnMenuAccel(id, accel);
}

void LRContainer::UpdateFocus(){
    if (cursorPos.x < dis){
        if (focus != lWindow)
            lWindow->OnFocus();
        focus = lWindow;
        right = false;
    }else{
        if (focus != rWindow)
            rWindow->OnFocus();
        focus = rWindow;
        right = true;
    }
    focus->OnMouseMove(right ? cursorPos.x - dis : cursorPos.x, cursorPos.y);
    DebugLog("LRContainer::focus %p", focus);
}

void LRContainer::FreeWindow(){
    delete lWindow;
    delete rWindow;
}

IWindow* LRContainer::GetLeftWindow(){
    return lWindow;
}

IWindow* LRContainer::GetRightWindow(){
    return rWindow;
}