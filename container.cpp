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
    if (x == 0 && y == 0){
        return;
    }
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
    if (dragEnable && Abs(x - dis) < 4.0f){
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
    adjustPos = false;
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
    if (focus)
        focus->OnKillFocus();
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

void LRContainer::OnDropFileA(const char* path){
    if (focus)
        focus->OnDropFileA(path);
}

void LRContainer::OnDropFileW(const wchar_t* path){
    if (focus)
        focus->OnDropFileW(path);
}

void LRContainer::UpdateFocus(){
    if (cursorPos.x < dis){
        if (focus != lWindow){
            if (focus)
                focus->OnKillFocus();
            lWindow->OnFocus();
        }
        focus = lWindow;
        right = false;
    }else{
        if (focus != rWindow){
            if (focus)
                focus->OnKillFocus();
            rWindow->OnFocus();
        }
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

void LRContainer::EnableDrag(){
    dragEnable = true;
}

void LRContainer::DisableDrag(){
    dragEnable = false;
}

bool LRContainer::DragEnabled(){
    return dragEnable;
}

UDContainer::UDContainer(IWindow* uWindow, IWindow* dWindow) : uWindow(uWindow), dWindow(dWindow) {}
UDContainer::~UDContainer(){}

bool UDContainer::IsFocus(){
    return focus != NULL;
}

void UDContainer::OnRender(){
    RECT tmp;

    tmp = ViewportManager::inst->GetCurrentRect();
    tmp.bottom = tmp.bottom + dis;
    ViewportManager::inst->PushViewport(tmp);
    uWindow->OnRender();
    ViewportManager::inst->PopViewport();

    tmp = ViewportManager::inst->GetCurrentRect();
    tmp.top = tmp.bottom + dis;
    ViewportManager::inst->PushViewport(tmp);
    dWindow->OnRender();
    ViewportManager::inst->PopViewport();
}

void UDContainer::OnCreate(HWND hWnd){
    uWindow->OnCreate(hWnd);
    dWindow->OnCreate(hWnd);
}

void UDContainer::OnClose(){
    uWindow->OnClose();
    dWindow->OnClose();
}

void UDContainer::OnResize(int x, int y){
    if (x == 0 && y == 0){
        return;
    }
    size.x = x;
    size.y = y;
    dis = GLUtils::Clamp(dis, 0.0f, size.y);
    uWindow->OnResize(x, y - dis);
    dWindow->OnResize(x, dis);
}

void UDContainer::OnMouseMove(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    if (adjustPos){
        SetCursor(LoadCursor(NULL, IDC_SIZENS));
        dis = y;
        uWindow->OnResize(size.x, size.y - dis);
        dWindow->OnResize(size.x, dis);
        return;
    }
    if (focus)
        focus->OnMouseMove(x, up ? y - dis : y);
}

void UDContainer::OnLeftDown(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    if (dragEnable && Abs(y - dis) < 4.0f){
        adjustPos = true;
        SetCursor(LoadCursor(NULL, IDC_SIZEWE));
        return;
    }
    UpdateFocus();
    if (focus)
        focus->OnLeftDown(x, up ? y - dis : y);
}

void UDContainer::OnLeftUp(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    adjustPos = false;
    if (focus)
        focus->OnLeftUp(x, up ? y - dis : y);
}

void UDContainer::OnRightDown(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    if (focus)
        focus->OnRightDown(x, up ? y - dis : y);
}

void UDContainer::OnRightUp(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    if (focus)
        focus->OnRightUp(x, up ? y - dis : y);
}

void UDContainer::OnMouseHover(int key, int x, int y){
    if (focus)
        focus->OnMouseHover(key, x, up ? y - dis : y);
}

void UDContainer::OnMouseLeave(){
    if (focus)
        focus->OnMouseLeave();
}

void UDContainer::OnFocus(){
    UpdateFocus();
}

void UDContainer::OnKillFocus(){
    if (focus)
        focus->OnKillFocus();
    focus = NULL;
    DebugLog("LRContainer::focus %p", focus);
}

void UDContainer::OnMouseWheel(int delta){
    if (focus)
        focus->OnMouseWheel(delta);
}

void UDContainer::OnMenuAccel(int id, bool accel){
    if (focus)
        focus->OnMenuAccel(id, accel);
}

void UDContainer::OnDropFileA(const char* path){
    if (focus)
        focus->OnDropFileA(path);
}

void UDContainer::OnDropFileW(const wchar_t* path){
    if (focus)
        focus->OnDropFileW(path);
}

void UDContainer::UpdateFocus(){
    if (cursorPos.y < dis){
        if (focus != dWindow){
            if (focus)
                focus->OnKillFocus();
            dWindow->OnFocus();
        }
        focus = dWindow;
        up = false;
    }else{
        if (focus != uWindow){
            if (focus)
                focus->OnKillFocus();
            uWindow->OnFocus();
        }
        focus = uWindow;
        up = true;
    }
    focus->OnMouseMove(cursorPos.x, up ? cursorPos.y - dis : cursorPos.y);
    DebugLog("UDContainer::focus %p", focus);
}

void UDContainer::FreeWindow(){
    delete uWindow;
    delete dWindow;
}

IWindow* UDContainer::GetUpWindow(){
    return uWindow;
}

IWindow* UDContainer::GetDownWindow(){
    return dWindow;
}

void UDContainer::EnableDrag(){
    dragEnable = true;
}

void UDContainer::DisableDrag(){
    dragEnable = false;
}

bool UDContainer::DragEnabled(){
    return dragEnable;
}