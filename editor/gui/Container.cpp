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
#include <editor/gui/Menu.h>
#include <editor/gui/ViewManager.h>
#include <utils/math3d/Math.h>
#include <utils/os/Log.h>
#include <utils/os/AppFrame.h>
#include <utils/gl/GLUtils.h>

LRContainer::LRContainer(IWindow* lWindow, IWindow* rWindow) : lWindow(lWindow), rWindow(rWindow) {}

LRContainer::LRContainer(IWindow* lWindow, IWindow* rWindow, SelectionWindow* selWindow) : selWindow(selWindow) {
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

    joinMenu->AddItem(new MenuItem(L"归并为左窗口", MENUITEM_LAMBDA_TRANS(LRContainer)[](LRContainer* window){
        if (window->selWindow){
            delete window->rWindow;
            window->rWindow = NULL;
            IWindow* curWindow = ((SelectionWindow*)window->lWindow)->GetWindow();
            ((SelectionWindow*)window->lWindow)->SetWindow(NULL, false);
            delete window->lWindow;
            window->lWindow = NULL;
            window->selWindow->SetWindow(curWindow, true);
        }
    }, this));
    joinMenu->AddItem(new MenuItem(L"归并为右窗口", MENUITEM_LAMBDA_TRANS(LRContainer)[](LRContainer* window){
        if (window->selWindow){
            delete window->lWindow;
            window->lWindow = NULL;
            IWindow* curWindow = ((SelectionWindow*)window->rWindow)->GetWindow();
            ((SelectionWindow*)window->rWindow)->SetWindow(NULL, false);
            delete window->rWindow;
            window->rWindow = NULL;
            window->selWindow->SetWindow(curWindow, true);
        }
    }, this));
}

bool LRContainer::IsFocus(){
    return focus != NULL;
}

void LRContainer::OnRender(){
    ViewManager* viewMgr = ViewManager::GetLocalInst();
    GLRect rect;

    rect = viewMgr->GetRect();
    rect.right = rect.left + dis;
    viewMgr->PushView(rect);
    if (lWindow){
        lWindow->OnRender();
    }else{
        GLUtils::Clear3DViewport();
    }
    viewMgr->PopView();

    rect = viewMgr->GetRect();
    rect.left = rect.left + dis;
    viewMgr->PushView(rect);
    if (rWindow){
        rWindow->OnRender();
    }else{
        GLUtils::Clear3DViewport();
    }
    viewMgr->PopView();
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
    dis = Round(Clamp(dis * x / size.x, 0.0f, (float)x));
    size.x = x;
    size.y = y;
    if (lWindow) lWindow->OnResize(dis, y);
    if (rWindow) rWindow->OnResize(x - dis, y);
}

void LRContainer::OnMouseMove(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    if (adjustPos){
        Main::SetCursor(IDC_SIZEWE);
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
        Main::SetCursor(IDC_SIZEWE);
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
    if (selWindow && Abs(x - dis) < 4.0f){
        if (joinMenu){
            Main::SetMenu(joinMenu);
        }else{
            DebugError("LRContainer::OnRightDown Main::SetMenu NullPointerException");
        }
        return;
    }
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
    //DebugLog("LRContainer::focus %p", focus);
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
    //DebugLog("LRContainer::focus %p", focus);
}

void LRContainer::FreeWindow(){
    if (lWindow){
        delete lWindow;
        lWindow = NULL;
    }
    if (rWindow){
        delete rWindow;
        rWindow = NULL;
    }
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

UDContainer::UDContainer(IWindow* uWindow, IWindow* dWindow, SelectionWindow* selWindow) : selWindow(selWindow) {
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

    joinMenu->AddItem(new MenuItem(L"归并为上窗口", MENUITEM_LAMBDA_TRANS(UDContainer)[](UDContainer* window){
        if (window->selWindow){
            delete window->dWindow;
            window->dWindow = NULL;
            IWindow* curWindow = ((SelectionWindow*)window->uWindow)->GetWindow();
            ((SelectionWindow*)window->uWindow)->SetWindow(NULL, false);
            delete window->uWindow;
            window->uWindow = NULL;
            window->selWindow->SetWindow(curWindow, true);
        }
    }, this));
    joinMenu->AddItem(new MenuItem(L"归并为下窗口", MENUITEM_LAMBDA_TRANS(UDContainer)[](UDContainer* window){
        if (window->selWindow){
            delete window->uWindow;
            window->uWindow = NULL;
            IWindow* curWindow = ((SelectionWindow*)window->dWindow)->GetWindow();
            ((SelectionWindow*)window->dWindow)->SetWindow(NULL, false);
            delete window->dWindow;
            window->dWindow = NULL;
            window->selWindow->SetWindow(curWindow, true);
        }
    }, this));
}

bool UDContainer::IsFocus(){
    return focus != NULL;
}

void UDContainer::OnRender(){
    ViewManager* viewMgr = ViewManager::GetLocalInst();
    GLRect rect;

    rect = viewMgr->GetRect();
    rect.bottom = rect.bottom + dis;
    viewMgr->PushView(rect);
    if (uWindow){
        uWindow->OnRender();
    }else{
        GLUtils::Clear3DViewport();
    }
    viewMgr->PopView();

    rect = viewMgr->GetRect();
    rect.top = rect.bottom + dis;
    viewMgr->PushView(rect);
    if (dWindow){
        dWindow->OnRender();
    }else{
        GLUtils::Clear3DViewport();
    }
    viewMgr->PopView();
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
    dis = Round(Clamp(dis * y / size.y, 0.0f, (float)y));
    size.x = x;
    size.y = y;
    if (uWindow) uWindow->OnResize(x, y - dis);
    if (dWindow) dWindow->OnResize(x, dis);
}

void UDContainer::OnMouseMove(int x, int y){
    cursorPos.x = x;
    cursorPos.y = y;
    if (adjustPos){
        Main::SetCursor(IDC_SIZENS);
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
        Main::SetCursor(IDC_SIZENS);
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
    if (selWindow && Abs(y - dis) < 4.0f){
        if (joinMenu){
            Main::SetMenu(joinMenu);
        }else{
            DebugError("UDContainer::OnRightDown Main::SetMenu NullPointerException");
        }
        return;
    }
    UpdateFocus();
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
    //DebugLog("UDContainer::focus %p", focus);
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
    //DebugLog("UDContainer::focus %p", focus);
}

void UDContainer::FreeWindow(){
    if (uWindow){
        delete uWindow;
        uWindow = NULL;
    }
    if (dWindow){
        delete dWindow;
        dWindow = NULL;
    }
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

SelectionWindow::SelectionWindow() : curWindow(NULL) {
    DebugLog("SelectionWindow %p Created", this);
    InitMenu();
}

SelectionWindow::SelectionWindow(IWindow* initialWnd) : curWindow(initialWnd) {
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

    selMenu->AddItem(new MenuItem(L"主窗口", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new MainWindow());
    }, this));
    selMenu->AddItem(new MenuItem(L"UV编辑器", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new UVEditWindow());
    }, this));
    selMenu->AddItem(new MenuItem(L"绘画窗口", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new PaintWindow());
    }, this));
    selMenu->AddItem(new MenuItem(L"音频播放器", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new AudioPlayerWindow());
    }, this));
    selMenu->AddItem(new MenuItem(L"变声器", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new AudioCaptureWindow());
    }, this));
    selMenu->AddItem(new MenuItem(L"树形图", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new TreeWindow());
    }, this));
    selMenu->AddItem(new MenuItem(L"节点编辑器", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new NodeMapWindow());
    }, this));
    selMenu->AddItem(new MenuItem(L"动画控制器", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new AnimationWindow());
    }, this));
    selMenu->AddItem(new MenuItem(L"渲染窗口", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new RenderWindow());
    }, this));
    selMenu->AddItem(new MenuItem());
    selMenu->AddItem(new MenuItem(L"左右分割至左侧", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new LRContainer(window->curWindow, NULL, window), false);
    }, this));
    selMenu->AddItem(new MenuItem(L"左右分割至右侧", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new LRContainer(NULL, window->curWindow, window), false);
    }, this));
    selMenu->AddItem(new MenuItem(L"上下分割至上侧", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new UDContainer(window->curWindow, NULL, window), false);
    }, this));
    selMenu->AddItem(new MenuItem(L"上下分割至下侧", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        window->SetWindow(new UDContainer(NULL, window->curWindow, window), false);
    }, this));
#ifdef PLATFORM_WINDOWS
    selMenu->AddItem(new MenuItem());
    selMenu->AddItem(new MenuItem(L"新窗口", MENUITEM_LAMBDA_TRANS(SelectionWindow)[](SelectionWindow* window){
        new AppFrame("ModelView New", new SelectionWindow(), 400, 400, true);
    }, this));
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
        GLUtils::Clear3DViewport();
    }
}

void SelectionWindow::OnCreate(){
    if (curWindow)
        curWindow->OnCreate();
}

void SelectionWindow::OnClose(){
    if (curWindow)
        curWindow->OnClose();
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
    sizeX = x; sizeY = y;
    if (curWindow)
        curWindow->OnResize(x, y);
}

void SelectionWindow::OnMouseMove(int x, int y){
    cursorX = x; cursorY = y;
    if (curWindow)
        curWindow->OnMouseMove(x, y);
}

void SelectionWindow::OnLeftDown(int x, int y){
    if (curWindow)
        curWindow->OnLeftDown(x, y);
}

void SelectionWindow::OnLeftUp(int x, int y){
    if (curWindow)
        curWindow->OnLeftUp(x, y);
}

void SelectionWindow::OnRightDown(int x, int y){
    if (curWindow)
        curWindow->OnRightDown(x, y);
}

void SelectionWindow::OnRightUp(int x, int y){
    if (curWindow)
        curWindow->OnRightUp(x, y);
}

void SelectionWindow::OnMouseHover(int key, int x, int y){
    if (curWindow)
        curWindow->OnMouseHover(key, x, y);
}

void SelectionWindow::OnMouseLeave(){
    if (curWindow)
        curWindow->OnMouseLeave();
}

void SelectionWindow::OnFocus(){
    if (curWindow)
        curWindow->OnFocus();
}

void SelectionWindow::OnKillFocus(){
    if (curWindow)
        curWindow->OnKillFocus();
}

void SelectionWindow::OnMouseWheel(int delta){
    if (curWindow)
        curWindow->OnMouseWheel(delta);
}

void SelectionWindow::OnMenuAccel(int id, bool accel){
    if (id == IDM_MENU_WINDOW){
        Main::SetMenu(selMenu);
    }
    if (curWindow)
        curWindow->OnMenuAccel(id, accel);
}

void SelectionWindow::OnDropFileA(const char* path){
    if (curWindow)
        curWindow->OnDropFileA(path);
}

void SelectionWindow::OnDropFileW(const wchar_t* path){
    if (curWindow)
        curWindow->OnDropFileW(path);
}

IWindow* SelectionWindow::GetWindow(){
    return curWindow;
}

void SelectionWindow::SetWindow(IWindow* window, bool del){
    if (del){
        if (curWindow){
            curWindow->OnClose();
            delete curWindow;
        }
        curWindow = window;
        if (curWindow){
            curWindow->OnCreate();
            curWindow->OnResize(sizeX, sizeY);
            curWindow->OnMouseMove(cursorX, cursorY);
        }
    }else{
        curWindow = window;
        if (curWindow){
            curWindow->OnResize(sizeX, sizeY);
            curWindow->OnMouseMove(cursorX, cursorY);
        }
    }
}