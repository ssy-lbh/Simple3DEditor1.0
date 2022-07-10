#include <editor/gui/Container.h>

#include <main.h>
#include <res.h>
#include <editor/AudioPlayerWindow.h>
#include <editor/AudioCaptureWindow.h>
#include <editor/AnimationWindow.h>
#include <editor/MainWindow.h>
#include <editor/NodeMapWindow.h>
#include <editor/PaintWindow.h>
#include <editor/TreeWindow.h>
#include <editor/UVEditWindow.h>
#include <editor/RenderWindow.h>
#include <editor/AttributeWindow.h>
#include <editor/gui/Menu.h>
#include <editor/main/ViewManager.h>
#include <util/Utils.h>
#include <util/math3d/Math.h>
#include <util/math3d/LinearAlgebra.h>
#include <util/gl/GLUtils.h>
#include <util/os/Log.h>
#include <util/os/AppFrame.h>

namespace simple3deditor {

AContainer::AContainer(){}
AContainer::AContainer(SelectionWindow* selWindow) : selWindow(selWindow) {}

LRContainer::LRContainer() : lWindow(nullptr), rWindow(nullptr) {
    cliSize = INIT_SIZE;
}

LRContainer::LRContainer(AWindow* lWindow, AWindow* rWindow) : lWindow(lWindow), rWindow(rWindow) {
    cliSize = INIT_SIZE;
}

LRContainer::LRContainer(AWindow* lWindow, AWindow* rWindow, SelectionWindow* selWindow) : AContainer(selWindow) {
    cliSize = INIT_SIZE;
    this->lWindow = new SelectionWindow(lWindow);
    this->rWindow = new SelectionWindow(rWindow);
    InitMenu();
}

LRContainer::~LRContainer(){
    if (joinMenu) delete joinMenu;
    FreeWindow();
}

void LRContainer::InitMenu(){
    joinMenu = new Menu();

    joinMenu->AddItem(new MenuItem(L"归并为左窗口", [=]{
        if (this->selWindow){
            delete this->rWindow;
            this->rWindow = nullptr;
            AWindow* curWindow = ((SelectionWindow*)this->lWindow)->GetWindow();
            ((SelectionWindow*)this->lWindow)->SetWindow(nullptr, false);
            delete this->lWindow;
            this->lWindow = nullptr;
            this->selWindow->SetWindow(curWindow, true);
        }
    }));
    joinMenu->AddItem(new MenuItem(L"归并为右窗口", [=]{
        if (this->selWindow){
            delete this->lWindow;
            this->lWindow = nullptr;
            AWindow* curWindow = ((SelectionWindow*)this->rWindow)->GetWindow();
            ((SelectionWindow*)this->rWindow)->SetWindow(nullptr, false);
            delete this->rWindow;
            this->rWindow = nullptr;
            this->selWindow->SetWindow(curWindow, true);
        }
    }));
}

bool LRContainer::IsFocus(){
    return focusWindow != nullptr;
}

void LRContainer::OnRender(){
    ViewManager* viewMgr = ViewManager::GetLocalInst();
    Rect rect;

    rect = viewMgr->GetRect();
    rect.right = rect.left + dis;
    viewMgr->PushView(rect);
    if (lWindow){
        lWindow->OnRender();
    }else{
        GLUtils::Clear2DViewport();
    }
    viewMgr->PopView();

    rect = viewMgr->GetRect();
    rect.left = rect.left + dis;
    viewMgr->PushView(rect);
    if (rWindow){
        rWindow->OnRender();
    }else{
        GLUtils::Clear2DViewport();
    }
    viewMgr->PopView();
}

void LRContainer::OnTimer(int id){
    if (lWindow) lWindow->OnTimer(id);
    if (rWindow) rWindow->OnTimer(id);
}

void LRContainer::OnChar(char c){
    if (focusWindow)
        focusWindow->OnChar(c);
}

void LRContainer::OnUnichar(wchar_t c){
    if (focusWindow)
        focusWindow->OnUnichar(c);
}

void LRContainer::OnResize(int x, int y){
    if (x == 0 || y == 0)
        return;
    dis = Round(Clamp(dis * x / cliSize.x, 0.0f, (float)x));
    AWindow::OnResize(x, y);
    if (lWindow) lWindow->OnResize(dis, y);
    if (rWindow) rWindow->OnResize(x - dis, y);
}

void LRContainer::OnMouseMove(int x, int y){
    AWindow::OnMouseMove(x, y);
    if (adjustPos){
        Main::SetCursor(IDC_SIZEWE);
        dis = x;
        if (lWindow) lWindow->OnResize(dis, cliSize.y);
        if (rWindow) rWindow->OnResize(cliSize.x - dis, cliSize.y);
        return;
    }
    if (focusWindow)
        focusWindow->OnMouseMove(right ? x - dis : x, y);
}

void LRContainer::OnLeftDown(int x, int y){
    AWindow::OnLeftDown(x, y);
    if (dragEnable && Abs(x - dis) < 4.0f){
        adjustPos = true;
        Main::SetCursor(IDC_SIZEWE);
        return;
    }
    UpdateFocus();
    if (focusWindow)
        focusWindow->OnLeftDown(right ? x - dis : x, y);
}

void LRContainer::OnLeftUp(int x, int y){
    AWindow::OnLeftUp(x, y);
    adjustPos = false;
    if (focusWindow)
        focusWindow->OnLeftUp(right ? x - dis : x, y);
}

void LRContainer::OnRightDown(int x, int y){
    AWindow::OnRightDown(x, y);
    if (selWindow && Abs(x - dis) < 4.0f){
        if (joinMenu){
            Main::SetMenu(joinMenu);
        }else{
            DebugError("LRContainer::OnRightDown Main::SetMenu NullPointerException");
        }
        return;
    }
    UpdateFocus();
    if (focusWindow)
        focusWindow->OnRightDown(right ? x - dis : x, y);
}

void LRContainer::OnRightUp(int x, int y){
    AWindow::OnRightUp(x, y);
    if (focusWindow)
        focusWindow->OnRightUp(right ? x - dis : x, y);
}

void LRContainer::OnFocus(){
    AWindow::OnFocus();
    UpdateFocus();
}

void LRContainer::OnKillFocus(){
    AWindow::OnKillFocus();
    if (focusWindow)
        focusWindow->OnKillFocus();
    focusWindow = nullptr;
    //DebugLog("LRContainer::focusWindow %p", focusWindow);
}

void LRContainer::OnMouseWheel(int delta){
    AWindow::OnMouseWheel(delta);
    if (focusWindow)
        focusWindow->OnMouseWheel(delta);
}

void LRContainer::OnMenuAccel(int id, bool accel){
    AWindow::OnMenuAccel(id, accel);
    if (focusWindow)
        focusWindow->OnMenuAccel(id, accel);
}

void LRContainer::OnDropFileA(const char* path, uint len){
    AWindow::OnDropFileA(path, len);
    if (focusWindow)
        focusWindow->OnDropFileA(path, len);
}

void LRContainer::OnDropFileW(const wchar_t* path, uint len){
    AWindow::OnDropFileW(path, len);
    if (focusWindow)
        focusWindow->OnDropFileW(path, len);
}

void LRContainer::Serialize(json& o){
    o["id"] = WINDOW_ID;
    o["div rate"] = GetRateClamped((float)dis, 0.0f, cliSize.x);
    if (lWindow)
        lWindow->Serialize(o["left window"]);
    if (rWindow)
        rWindow->Serialize(o["right window"]);
}

void LRContainer::Deserialize(json& o){
    dis = LerpClamped(0.0f, cliSize.x, o.value("div rate", 0.5f));
    lWindow = o.contains("left window") ? Main::data->ConstructWindow(o["left window"]) : NULL;
    rWindow = o.contains("right window") ? Main::data->ConstructWindow(o["right window"]) : NULL;
}

void LRContainer::UpdateFocus(){
    if (cursorCoord.x < dis){
        if (focusWindow != lWindow){
            if (focusWindow)
                focusWindow->OnKillFocus();
            if (lWindow)
                lWindow->OnFocus();
        }
        focusWindow = lWindow;
        right = false;
    }else{
        if (focusWindow != rWindow){
            if (focusWindow)
                focusWindow->OnKillFocus();
            if (rWindow)
                rWindow->OnFocus();
        }
        focusWindow = rWindow;
        right = true;
    }
    if (focusWindow)
        focusWindow->OnMouseMove(right ? cursorCoord.x - dis : cursorCoord.x, cursorCoord.y);
    //DebugLog("LRContainer::focusWindow %p", focusWindow);
}

void LRContainer::FreeWindow(){
    if (lWindow){
        delete lWindow;
        lWindow = nullptr;
    }
    if (rWindow){
        delete rWindow;
        rWindow = nullptr;
    }
}

AWindow* LRContainer::GetLeftWindow(){
    return lWindow;
}

AWindow* LRContainer::GetRightWindow(){
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

UDContainer::UDContainer() : uWindow(nullptr), dWindow(nullptr) {
    cliSize = INIT_SIZE;
} 

UDContainer::UDContainer(AWindow* uWindow, AWindow* dWindow) : uWindow(uWindow), dWindow(dWindow) {
    cliSize = INIT_SIZE;
}

UDContainer::UDContainer(AWindow* uWindow, AWindow* dWindow, SelectionWindow* selWindow) : AContainer(selWindow) {
    cliSize = INIT_SIZE;
    this->uWindow = new SelectionWindow(uWindow);
    this->dWindow = new SelectionWindow(dWindow);
    InitMenu();
}

UDContainer::~UDContainer(){
    if (joinMenu) delete joinMenu;
    FreeWindow();
}

void UDContainer::InitMenu(){
    joinMenu = new Menu();

    joinMenu->AddItem(new MenuItem(L"归并为上窗口", [=]{
        if (this->selWindow){
            delete this->dWindow;
            this->dWindow = nullptr;
            AWindow* curWindow = ((SelectionWindow*)this->uWindow)->GetWindow();
            ((SelectionWindow*)this->uWindow)->SetWindow(nullptr, false);
            delete this->uWindow;
            this->uWindow = nullptr;
            this->selWindow->SetWindow(curWindow, true);
        }
    }));
    joinMenu->AddItem(new MenuItem(L"归并为下窗口", [=]{
        if (this->selWindow){
            delete this->uWindow;
            this->uWindow = nullptr;
            AWindow* curWindow = ((SelectionWindow*)this->dWindow)->GetWindow();
            ((SelectionWindow*)this->dWindow)->SetWindow(nullptr, false);
            delete this->dWindow;
            this->dWindow = nullptr;
            this->selWindow->SetWindow(curWindow, true);
        }
    }));
}

bool UDContainer::IsFocus(){
    return focusWindow != nullptr;
}

void UDContainer::OnRender(){
    ViewManager* viewMgr = ViewManager::GetLocalInst();
    Rect rect;

    rect = viewMgr->GetRect();
    rect.bottom = rect.bottom + dis;
    viewMgr->PushView(rect);
    if (uWindow){
        uWindow->OnRender();
    }else{
        GLUtils::Clear2DViewport();
    }
    viewMgr->PopView();

    rect = viewMgr->GetRect();
    rect.top = rect.bottom + dis;
    viewMgr->PushView(rect);
    if (dWindow){
        dWindow->OnRender();
    }else{
        GLUtils::Clear2DViewport();
    }
    viewMgr->PopView();
}

void UDContainer::OnTimer(int id){
    if (uWindow) uWindow->OnTimer(id);
    if (dWindow) dWindow->OnTimer(id);
}

void UDContainer::OnChar(char c){
    if (focusWindow)
        focusWindow->OnChar(c);
}

void UDContainer::OnUnichar(wchar_t c){
    if (focusWindow)
        focusWindow->OnUnichar(c);
}

void UDContainer::OnResize(int x, int y){
    if (x == 0 || y == 0)
        return;
    dis = Round(Clamp(dis * y / cliSize.y, 0.0f, (float)y));
    AWindow::OnResize(x, y);
    if (uWindow) uWindow->OnResize(x, y - dis);
    if (dWindow) dWindow->OnResize(x, dis);
}

void UDContainer::OnMouseMove(int x, int y){
    AWindow::OnMouseMove(x, y);
    if (adjustPos){
        Main::SetCursor(IDC_SIZENS);
        dis = y;
        if (uWindow) uWindow->OnResize(cliSize.x, cliSize.y - dis);
        if (dWindow) dWindow->OnResize(cliSize.x, dis);
        return;
    }
    if (focusWindow)
        focusWindow->OnMouseMove(x, up ? y - dis : y);
}

void UDContainer::OnLeftDown(int x, int y){
    AWindow::OnLeftDown(x, y);
    if (dragEnable && Abs(y - dis) < 4.0f){
        adjustPos = true;
        Main::SetCursor(IDC_SIZENS);
        return;
    }
    UpdateFocus();
    if (focusWindow)
        focusWindow->OnLeftDown(x, up ? y - dis : y);
}

void UDContainer::OnLeftUp(int x, int y){
    AWindow::OnLeftUp(x, y);
    adjustPos = false;
    if (focusWindow)
        focusWindow->OnLeftUp(x, up ? y - dis : y);
}

void UDContainer::OnRightDown(int x, int y){
    AWindow::OnRightDown(x, y);
    if (selWindow && Abs(y - dis) < 4.0f){
        if (joinMenu){
            Main::SetMenu(joinMenu);
        }else{
            DebugError("UDContainer::OnRightDown Main::SetMenu NullPointerException");
        }
        return;
    }
    UpdateFocus();
    if (focusWindow)
        focusWindow->OnRightDown(x, up ? y - dis : y);
}

void UDContainer::OnRightUp(int x, int y){
    AWindow::OnRightUp(x, y);
    if (focusWindow)
        focusWindow->OnRightUp(x, up ? y - dis : y);
}

void UDContainer::OnFocus(){
    AWindow::OnFocus();
    UpdateFocus();
}

void UDContainer::OnKillFocus(){
    AWindow::OnKillFocus();
    if (focusWindow)
        focusWindow->OnKillFocus();
    focusWindow = nullptr;
}

void UDContainer::OnMouseWheel(int delta){
    AWindow::OnMouseWheel(delta);
    if (focusWindow)
        focusWindow->OnMouseWheel(delta);
}

void UDContainer::OnMenuAccel(int id, bool accel){
    AWindow::OnMenuAccel(id, accel);
    if (focusWindow)
        focusWindow->OnMenuAccel(id, accel);
}

void UDContainer::OnDropFileA(const char* path, uint len){
    AWindow::OnDropFileA(path, len);
    if (focusWindow)
        focusWindow->OnDropFileA(path, len);
}

void UDContainer::OnDropFileW(const wchar_t* path, uint len){
    AWindow::OnDropFileW(path, len);
    if (focusWindow)
        focusWindow->OnDropFileW(path, len);
}

void UDContainer::Serialize(json& o){
    o["id"] = WINDOW_ID;
    o["div rate"] = GetRateClamped((float)dis, 0.0f, cliSize.y);
    if (uWindow)
        uWindow->Serialize(o["up window"]);
    if (dWindow)
        dWindow->Serialize(o["down window"]);
}

void UDContainer::Deserialize(json& o){
    dis = LerpClamped(0.0f, cliSize.y, o.value("div rate", 0.5f));
    uWindow = o.contains("up window") ? Main::data->ConstructWindow(o["up window"]) : NULL;
    dWindow = o.contains("down window") ? Main::data->ConstructWindow(o["down window"]) : NULL;
}

void UDContainer::UpdateFocus(){
    if (cursorCoord.y < dis){
        if (focusWindow != dWindow){
            if (focusWindow)
                focusWindow->OnKillFocus();
            if (dWindow)
                dWindow->OnFocus();
        }
        focusWindow = dWindow;
        up = false;
    }else{
        if (focusWindow != uWindow){
            if (focusWindow)
                focusWindow->OnKillFocus();
            if (uWindow)
                uWindow->OnFocus();
        }
        focusWindow = uWindow;
        up = true;
    }
    if (focusWindow)
        focusWindow->OnMouseMove(cursorCoord.x, up ? cursorCoord.y - dis : cursorCoord.y);
    //DebugLog("UDContainer::focusWindow %p", focusWindow);
}

void UDContainer::FreeWindow(){
    if (uWindow){
        delete uWindow;
        uWindow = nullptr;
    }
    if (dWindow){
        delete dWindow;
        dWindow = nullptr;
    }
}

AWindow* UDContainer::GetUpWindow(){
    return uWindow;
}

AWindow* UDContainer::GetDownWindow(){
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

SelectionWindow::SelectionWindow() : curWindow(nullptr) {
    DebugLog("SelectionWindow %p Created", this);
    InitMenu();
}

SelectionWindow::SelectionWindow(AWindow* initialWnd) : curWindow(initialWnd) {
    DebugLog("SelectionWindow %p Created", this);
    InitMenu();
}

SelectionWindow::~SelectionWindow(){
    DebugLog("SelectionWindow %p Destroyed", this);
    if (selMenu) delete selMenu;
    if (curWindow) delete curWindow;
}

void SelectionWindow::InitMenu(){
    selMenu = new Menu();

    // 直接无序注册所有窗口
    Main::data->windowReg.Foreach([=](WindowInfo* info){
        if (info->displayName.GetLength() == 0)
            return;
        // 这里复制了name和factory存入lambda函数对象中
        WString name = info->displayName;
        std::function<AWindow*()> factory = info->factory;
        this->selMenu->AddItem(new MenuItem(
            Value<const wchar_t*>([name](){ return name.GetString(); }),
            [this, factory]{ this->SetWindow(factory()); }
        ));
    });

    selMenu->AddItem(new MenuItem());
    selMenu->AddItem(new MenuItem(L"左右分割至左侧", [=]{ this->SetWindow(new LRContainer(this->curWindow, NULL, this), false); }));
    selMenu->AddItem(new MenuItem(L"左右分割至右侧", [=]{ this->SetWindow(new LRContainer(NULL, this->curWindow, this), false); }));
    selMenu->AddItem(new MenuItem(L"上下分割至上侧", [=]{ this->SetWindow(new UDContainer(this->curWindow, NULL, this), false); }));
    selMenu->AddItem(new MenuItem(L"上下分割至下侧", [=]{ this->SetWindow(new UDContainer(NULL, this->curWindow, this), false); }));
#ifdef PLATFORM_WINDOWS
    selMenu->AddItem(new MenuItem());
    selMenu->AddItem(new MenuItem(L"新窗口", [=]{ new AppFrame("ModelView New", new SelectionWindow(), 400, 400, true); }));
#endif
}

bool SelectionWindow::IsFocus(){
    if (curWindow)
        return curWindow->IsFocus();
    return false;
}

void SelectionWindow::OnRender(){
    if (curWindow){
        curWindow->OnRender();
    }else{
        GLUtils::Clear2DViewport();
    }
}

void SelectionWindow::OnTimer(int id){
    if (curWindow)
        curWindow->OnTimer(id);
}

void SelectionWindow::OnChar(char c){
    if (curWindow)
        curWindow->OnChar(c);
}

void SelectionWindow::OnUnichar(wchar_t c){
    if (curWindow)
        curWindow->OnUnichar(c);
}

void SelectionWindow::OnResize(int x, int y){
    AWindow::OnResize(x, y);
    if (curWindow)
        curWindow->OnResize(x, y);
}

void SelectionWindow::OnMouseMove(int x, int y){
    AWindow::OnMouseMove(x, y);
    if (curWindow)
        curWindow->OnMouseMove(x, y);
}

void SelectionWindow::OnLeftDown(int x, int y){
    AWindow::OnLeftDown(x, y);
    if (curWindow)
        curWindow->OnLeftDown(x, y);
}

void SelectionWindow::OnLeftUp(int x, int y){
    AWindow::OnLeftUp(x, y);
    if (curWindow)
        curWindow->OnLeftUp(x, y);
}

void SelectionWindow::OnRightDown(int x, int y){
    AWindow::OnRightDown(x, y);
    if (curWindow)
        curWindow->OnRightDown(x, y);
}

void SelectionWindow::OnRightUp(int x, int y){
    AWindow::OnRightUp(x, y);
    if (curWindow)
        curWindow->OnRightUp(x, y);
}

void SelectionWindow::OnFocus(){
    AWindow::OnFocus();
    if (curWindow)
        curWindow->OnFocus();
}

void SelectionWindow::OnKillFocus(){
    AWindow::OnKillFocus();
    if (curWindow)
        curWindow->OnKillFocus();
}

void SelectionWindow::OnMouseWheel(int delta){
    AWindow::OnMouseWheel(delta);
    if (curWindow)
        curWindow->OnMouseWheel(delta);
}

void SelectionWindow::OnMenuAccel(int id, bool accel){
    AWindow::OnMenuAccel(id, accel);
    if (id == IDM_MENU_WINDOW){
        Main::SetMenu(selMenu);
    }
    if (curWindow)
        curWindow->OnMenuAccel(id, accel);
}

void SelectionWindow::OnDropFileA(const char* path, uint len){
    AWindow::OnDropFileA(path, len);
    if (curWindow)
        curWindow->OnDropFileA(path, len);
}

void SelectionWindow::OnDropFileW(const wchar_t* path, uint len){
    AWindow::OnDropFileW(path, len);
    if (curWindow)
        curWindow->OnDropFileW(path, len);
}

void SelectionWindow::Serialize(json& o){
    o["id"] = WINDOW_ID;
    if (curWindow)
        curWindow->Serialize(o["window"]);
}

void SelectionWindow::Deserialize(json& o){
    if (o.contains("window")){
        curWindow = Main::data->ConstructWindow(o["window"]);
        if (InstanceOf<AContainer>(curWindow))
            ((AContainer*)curWindow)->selWindow = this;
    } else {
        curWindow = NULL;
    }
}

AWindow* SelectionWindow::GetWindow(){
    return curWindow;
}

void SelectionWindow::SetWindow(AWindow* window, bool del){
    if (del){
        if (curWindow)
            delete curWindow;
        curWindow = window;
        if (curWindow){
            curWindow->OnResize(cliSize.x, cliSize.y);
            curWindow->OnMouseMove(cursorCoord.x, cursorCoord.y);
        }
    }else{
        curWindow = window;
        if (curWindow){
            curWindow->OnResize(cliSize.x, cliSize.y);
            curWindow->OnMouseMove(cursorCoord.x, cursorCoord.y);
        }
    }
}

}