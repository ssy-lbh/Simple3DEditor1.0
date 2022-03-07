#include <editor/RenderWindow.h>

#include <lib/opengl/gl/gl.h>
#include <lib/opengl/gl/glu.h>

#include <main.h>
#include <res.h>
#include <editor/gui/ViewManager.h>
#include <utils/String.h>
#include <utils/File.h>
#include <utils/DataBuffer.h>
#include <utils/os/Shell.h>
#include <utils/os/Resource.h>
#include <utils/math3d/ViewObject.h>

RenderWindow::RenderWindow(){
    DebugLog("RenderWindow Launched");
    uiMgr = new UIManager();

    basicMenu = new Menu();
    basicMenu->AddItem(new MenuItem(L"保存", MENUITEM_LAMBDA_TRANS(RenderWindow)[](RenderWindow* window){
        window->OnMenuAccel(IDM_SAVE, false);
    }, this));
    basicMenu->AddItem(new MenuItem(L"打印", MENUITEM_LAMBDA_TRANS(RenderWindow)[](RenderWindow* window){
        window->OnMenuAccel(IDM_PRINT, false);
    }, this));
    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new SwitchMenuItem(L"光照:开", L"光照:关", SWITCHMENUITEM_LAMBDA_TRANS(RenderWindow)[](bool state, RenderWindow* window){
        window->lightEnabled = state;
        DebugLog("RenderWindow Light State %s", window->lightEnabled ? "On" : "Off");
    }, this, lightEnabled));
}

RenderWindow::~RenderWindow(){
    DebugLog("RenderWindow Destroyed");
    if (basicMenu) delete basicMenu;
    if (uiMgr) delete uiMgr;
}

void RenderWindow::InitCamera(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, ViewManager::GetLocalInst()->GetAspect(), camDis * 0.02, camDis * 20.0);

    Vector3 camPos = camLookat - camDir * Vector3::forward * camDis;
    Vector3 camUp = camDir * Vector3::up;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPos.x, camPos.y, camPos.z,
            camLookat.x, camLookat.y, camLookat.z,
            camUp.x, camUp.y, camUp.z);
}

bool RenderWindow::IsFocus(){
    return focus;
}

void RenderWindow::OnRender(){
    ViewManager* viewMgr = ViewManager::GetLocalInst();

    rect = viewMgr->GetRect();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //TODO 后续自制渲染管线
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glDisable(GL_DITHER);
    glDisable(GL_FOG);
    glDisable(GL_LOGIC_OP);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    InitCamera();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    RenderOptions options;
    options.vertex = false;
    options.edge = false;
    options.face = true;
    options.light = lightEnabled;
    options.editor = false;

    Main::data->scene->OnChainRender(&options);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLUtils::ResetProjection();
    GLUtils::ResetModelView();

    uiMgr->Render();
}

void RenderWindow::OnClose(){}

void RenderWindow::OnTimer(int id){}

void RenderWindow::OnChar(char c){}

void RenderWindow::OnUnichar(wchar_t c){}

void RenderWindow::OnResize(int x, int y){
    UpdateWindowSize(x, y);
}

void RenderWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void RenderWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
}

void RenderWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
}

void RenderWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
    Main::SetMenu(basicMenu);
}

void RenderWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
}

void RenderWindow::OnMouseHover(int key, int x, int y){}

void RenderWindow::OnMouseLeave(){}

void RenderWindow::OnFocus(){
    focus = true;
}

void RenderWindow::OnKillFocus(){
    focus = false;
}

void RenderWindow::OnMouseWheel(int delta){}

void RenderWindow::OnMenuAccel(int id, bool accel){
    switch (id){
    case IDM_SAVE:
        OnInsSave();
        break;
    case IDM_PRINT:
        Main::SaveImage(".\\temp.png", rect);
        ShellPrint(WString(L".\\temp.png"));
        break;
    case IDM_RENDER_ANIMATION:
        Main::RenderAnimation("C:\\tmp\\", 0, 250, rect);
        break;
    }
}

void RenderWindow::OnDropFileA(const char* path){}

void RenderWindow::OnDropFileW(const wchar_t* path){}

void RenderWindow::OnInsSave(){
    WString file = ShellFileSelectWindow(WString(IDS_PICFILE_FILTER), FILESELECT_REQ_PATH, true);
    if (file.GetLength() == 0){
        DebugError("Stop Saving");
        return;
    }
    Main::SaveImage(file, rect);
}

void RenderWindow::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
}

void RenderWindow::UpdateCursor(int x, int y){
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 2.0f * y / cliSize.y - 1.0f;
}