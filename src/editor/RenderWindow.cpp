#include <editor/RenderWindow.h>

#include <lib/glew/gl/glew.h>

#include <main.h>
#include <res.h>
#include <util/String.h>
#include <io/File.h>
#include <util/DataBuffer.h>
#include <util/os/Shell.h>
#include <util/os/Resource.h>
#include <util/gl/GLUtils.h>
#include <util/gl/GLShader.h>
#include <util/gl/GLProgram.h>
#include <editor/gui/Menu.h>
#include <editor/main/ViewManager.h>
#include <editor/main/ViewObject.h>
#include <editor/object/GUIManagerObject.h>

namespace simple3deditor {

RenderWindow::RenderWindow() : CCamera(Point3(0.0f, -5.0f, 1.0f), Point3(0.0f, 0.0f, 1.0f), Vector3::up, 5.0f) {
    DebugLog("RenderWindow Launched");
    guiMgr = new GUIManagerObject();

    basicMenu = new Menu();
    basicMenu->AddItem(new MenuItem(L"保存", [=]{ this->OnMenuAccel(IDM_SAVE, false); }));
    basicMenu->AddItem(new MenuItem(L"打印", [=]{ this->OnMenuAccel(IDM_PRINT, false); }));
    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new MenuItem(
        Value<const wchar_t*>([=]{ return this->lightEnabled ? L"光照:开" : L"光照:关"; }),
        [=]{
            this->lightEnabled = !this->lightEnabled;
            DebugLog("RenderWindow Light State %s", this->lightEnabled ? "On" : "Off");
        }
    ));
}

RenderWindow::~RenderWindow(){
    DebugLog("RenderWindow Destroyed");
    if (basicMenu) delete basicMenu;
    if (guiMgr) delete guiMgr;
}

void RenderWindow::InitCamera(){
    CCamera::InitCamera(aspect);
}

void RenderWindow::OnRender(){
    ViewManager* viewMgr = ViewManager::GetLocalInst();

    rect = viewMgr->GetRect();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderOptions* options = &LocalData::GetLocalInst()->renderOptions;
    options->editor = false;
    options->vertex = false;
    options->edge = false;
    options->face = true;
    options->light = lightEnabled;
    
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
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    InitCamera();

    Main::RenderScene();

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLUtils::ResetProjection();
    GLUtils::ResetModelView();

    Main::RenderScreen();

    guiMgr->OnChainRender();
}

void RenderWindow::OnTimer(int id){}

void RenderWindow::OnChar(char c){}

void RenderWindow::OnUnichar(wchar_t c){}

void RenderWindow::OnResize(int x, int y){
    UpdateWindowSize(x, y);
}

void RenderWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnMouseMove2D(cursorPos);
}

void RenderWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnLeftDown2D(cursorPos);
}

void RenderWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnLeftUp2D(cursorPos);
}

void RenderWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnRightDown2D(cursorPos);
    Main::SetMenu(basicMenu);
}

void RenderWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnRightUp2D(cursorPos);
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

void RenderWindow::Serialize(IOutputStream& os){
    os.WriteWithLen(WINDOW_ID);
}

void RenderWindow::Deserialize(IInputStream& os){}

void RenderWindow::OnInsSave(){
    static const WString filter = Resource::GetWString(IDS_PICFILE_FILTER);
    WString file = ShellFileSelectWindow(filter, FILESELECT_REQ_PATH, true);
    if (file.GetLength() == 0){
        DebugError("Stop Saving");
        return;
    }
    Main::SaveImage(file, rect);
}

void RenderWindow::UpdateWindowSize(int x, int y){
    AWindow::UpdateWindowSize(x, y);
}

void RenderWindow::UpdateCursor(int x, int y){
    AWindow::UpdateCursor(x, y);
}

void RenderWindow::SetLookAt(Point3 at){
    CCamera::SetLookAt(at);
}

void RenderWindow::SetRotation(Quaternion rot){
    CCamera::SetRotation(rot);
}

void RenderWindow::SetDistance(float dis){
    CCamera::SetDistance(dis);
}

}