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
    if (lWindow){
        lWindow->OnRender();
    }else{
        GLUtils::Clear3DViewport();
    }
    ViewportManager::inst->PopViewport();

    tmp = ViewportManager::inst->GetCurrentRect();
    tmp.left = tmp.left + dis;
    ViewportManager::inst->PushViewport(tmp);
    if (rWindow){
        rWindow->OnRender();
    }else{
        GLUtils::Clear3DViewport();
    }
    ViewportManager::inst->PopViewport();
}

void LRContainer::OnCreate(){
    if (lWindow) lWindow->OnCreate();
    if (rWindow) rWindow->OnCreate();
}

void LRContainer::OnClose(){
    if (lWindow) lWindow->OnClose();
    if (rWindow) rWindow->OnClose();
}

void LRContainer::OnTimer(int id){
    if (lWindow) lWindow->OnTimer(id);
    if (rWindow) rWindow->OnTimer(id);
}

void LRContainer::OnChar(char c){
    if (focus)
        focus->OnChar(c);
}

void LRContainer::OnUnichar(wchar_t c){
    if (focus)
        focus->OnUnichar(c);
}

void LRContainer::OnResize(int x, int y){
    if (x == 0 && y == 0){
        return;
    }
    size.x = x;
    size.y = y;
    dis = GLUtils::Clamp(dis, 0.0f, size.x);
    if (lWindow) lWindow->OnResize(dis, y);
    if (rWindow) rWindow->OnResize(x - dis, y);
}

void LRContainer::OnMouseMove(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    if (adjustPos){
        SetCursor(LoadCursor(NULL, IDC_SIZEWE));
        dis = x;
        if (lWindow) lWindow->OnResize(dis, size.y);
        if (rWindow) rWindow->OnResize(size.x - dis, size.y);
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
    if (focus)
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
    if (focus)
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
            if (lWindow)
                lWindow->OnFocus();
        }
        focus = lWindow;
        right = false;
    }else{
        if (focus != rWindow){
            if (focus)
                focus->OnKillFocus();
            if (rWindow)
                rWindow->OnFocus();
        }
        focus = rWindow;
        right = true;
    }
    if (focus)
        focus->OnMouseMove(right ? cursorPos.x - dis : cursorPos.x, cursorPos.y);
    DebugLog("LRContainer::focus %p", focus);
}

void LRContainer::FreeWindow(){
    if (lWindow) delete lWindow;
    if (rWindow) delete rWindow;
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
    if (uWindow){
        uWindow->OnRender();
    }else{
        GLUtils::Clear3DViewport();
    }
    ViewportManager::inst->PopViewport();

    tmp = ViewportManager::inst->GetCurrentRect();
    tmp.top = tmp.bottom + dis;
    ViewportManager::inst->PushViewport(tmp);
    if (dWindow){
        dWindow->OnRender();
    }else{
        GLUtils::Clear3DViewport();
    }
    ViewportManager::inst->PopViewport();
}

void UDContainer::OnCreate(){
    if (uWindow) uWindow->OnCreate();
    if (dWindow) dWindow->OnCreate();
}

void UDContainer::OnClose(){
    if (uWindow) uWindow->OnClose();
    if (dWindow) dWindow->OnClose();
}

void UDContainer::OnTimer(int id){
    if (uWindow) uWindow->OnTimer(id);
    if (dWindow) dWindow->OnTimer(id);
}

void UDContainer::OnChar(char c){
    if (focus)
        focus->OnChar(c);
}

void UDContainer::OnUnichar(wchar_t c){
    if (focus)
        focus->OnUnichar(c);
}

void UDContainer::OnResize(int x, int y){
    if (x == 0 && y == 0){
        return;
    }
    size.x = x;
    size.y = y;
    dis = GLUtils::Clamp(dis, 0.0f, size.y);
    if (uWindow) uWindow->OnResize(x, y - dis);
    if (dWindow) dWindow->OnResize(x, dis);
}

void UDContainer::OnMouseMove(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    if (adjustPos){
        SetCursor(LoadCursor(NULL, IDC_SIZENS));
        dis = y;
        if (uWindow) uWindow->OnResize(size.x, size.y - dis);
        if (dWindow) dWindow->OnResize(size.x, dis);
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
        SetCursor(LoadCursor(NULL, IDC_SIZENS));
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
            if (dWindow)
                dWindow->OnFocus();
        }
        focus = dWindow;
        up = false;
    }else{
        if (focus != uWindow){
            if (focus)
                focus->OnKillFocus();
            if (uWindow)
                uWindow->OnFocus();
        }
        focus = uWindow;
        up = true;
    }
    if (focus)
        focus->OnMouseMove(cursorPos.x, up ? cursorPos.y - dis : cursorPos.y);
    DebugLog("UDContainer::focus %p", focus);
}

void UDContainer::FreeWindow(){
    if (uWindow) delete uWindow;
    if (dWindow) delete dWindow;
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