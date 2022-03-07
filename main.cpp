#include <main.h>

#include <lib/opengl/gl/gl.h>

#include <res.h>
#include <editor/gui/Container.h>
#include <editor/gui/Menu.h>
#include <editor/MainWindow.h>
#include <editor/dialog/Tips.h>
#include <utils/AudioUtils.h>
#include <utils/StringBuilder.h>
#include <utils/os/Log.h>
#include <utils/os/Font.h>
#include <utils/os/Thread.h>
#include <utils/os/Shell.h>
#include <utils/os/Resource.h>
#include <utils/gl/GLUtils.h>
#include <utils/math3d/ViewObject.h>

LocalData::LocalData(){}
LocalData::~LocalData(){}

LocalData* LocalData::GetLocalInst(){
    return (LocalData*)ThreadLocal::Get(THREAD_LOCAL_LOCALDATA);
}

void LocalData::UpdateCursor(int x, int y){
    // 坐标反转
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 1.0f - 2.0f * y / cliSize.y;
    if (menu)
        menu->CursorMove(cursorPos - menuPos);
}

void LocalData::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
    aspect = cliSize.x / cliSize.y;
    if (menu)
        menu->SetClientSize(cliSize);
}

void LocalData::SetMenu(Menu* m){
    if (menu)
        menu->ResetSelect();
    menu = m;
    if (menu){
        menu->SetClientSize(cliSize);
        menu->CursorMove(cursorPos);
        menuPos = cursorPos;
    }
}

void LocalData::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    if (menu){
        if (menu->InChainMenu(cursorPos - menuPos)){
            menu->Click();
        }
        SetMenu(NULL);
    }
}

void LocalData::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
}

void LocalData::OnRightDown(int x, int y){
    UpdateCursor(x, y);
}

void LocalData::OnRightUp(int x, int y){
    UpdateCursor(x, y);
}

void LocalData::OnMenuAccel(int id, bool accel){
    switch (id){
    case IDM_EXIT:
        if (ShellMsgBox(WString(IDS_EXIT_CONFIRM_CAPTION), WString(IDS_EXIT_CONFIRM)) == MSGBOX_YES)
            PostQuitMessage(0);
        break;
    case IDM_ABOUT:
        DialogVersionInfo();
        break;
    case IDM_LEFT:
        if (menu)
            menu->PressLeft();
        break;
    case IDM_RIGHT:
        if (menu)
            menu->PressRight();
        break;
    case IDM_UP:
        if (menu)
            menu->PressUp();
        break;
    case IDM_DOWN:
        if (menu)
            menu->PressDown();
        break;
    case IDM_CONFIRM:
        if (menu){
            menu->Click();
            SetMenu(NULL);
        }
        break;
    case IDM_CANCEL:
        SetMenu(NULL);
        break;
    }
}

void LocalData::Render(){
    if (menu)
        menu->Render(menuPos.x, menuPos.y);
}

GlobalData::GlobalData(){
    scene = new AViewObject(L"Scene");
}

GlobalData::~GlobalData(){
    if (scene) delete scene;
}

void GlobalData::SelectObject(AViewObject* o){
    if (!o){
        DebugError("Warning: GlobalData::SelectObject Object Is NULL");
        return;
    }
    curObject = o;
    selObjects.Clear();
    selPoints.Clear();
    selEdges.Clear();
    selFaces.Clear();
}

void GlobalData::OnAnimationFrame(float frame){
    animFrame = frame;
    scene->OnAnimationFrame(frame);
}

Main::Main(){
    MeshObject* mesh;

    data = new GlobalData();
    mesh = new MeshObject();
    
    AddObject(mesh);
    SelectObject(mesh);
}

Main::~Main(){
    if (data) delete data;
}

void Main::RequestRender(){
    AppFrame* frame = AppFrame::GetLocalInst();
    if (frame)
        frame->reqRender = true;
}

#ifdef PLATFORM_WINDOWS
void Main::SetCursor(int id){
    AppFrame* frame = AppFrame::GetLocalInst();
    ::SetCursor(LoadCursorA(GetModuleHandleA(NULL), MAKEINTRESOURCEA(id)));
    if (frame)
        frame->cursorSelected = true;
}

void Main::SetCursor(const char* res){
    AppFrame* frame = AppFrame::GetLocalInst();
    ::SetCursor(LoadCursorA(NULL, res));
    if (frame)
        frame->cursorSelected = true;
}
#endif

void Main::SetMenu(Menu* m){
    LocalData* data = LocalData::GetLocalInst();
    if (data)
        data->SetMenu(m);
}

void Main::SelectObject(AViewObject* o){
    data->SelectObject(o);
}

void Main::AddObject(AViewObject* o){
    data->scene->AddChild(o);
}

void Main::DeleteObject(AViewObject* o){
    if (o == data->scene){
        DebugError("Main::DeleteObject Trying To Delete Scene Object");
        return;
    }
    delete o;
    if (o == data->curObject)
        data->SelectObject(data->scene);
}

void Main::OnAnimationFrame(float frame){
    data->OnAnimationFrame(frame);
    RequestRender();
}

void Main::SaveImage(String file, GLRect rect){
    size_t width = (size_t)rect.GetWidth();
    size_t height = (size_t)rect.GetHeight();
    size_t size = (width * height) << 2;
    char* buffer = new char[size];

    DebugLog("SaveImage %s", file.GetString());

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(rect.left, rect.bottom, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    Resource::StoreImage(file, buffer, width, height, 4);

    delete[] buffer;
}

void Main::RenderAnimation(String dir, size_t start, size_t end, GLRect rect){
    AppFrame* frame = AppFrame::GetLocalInst();
    StringBuilder builder;

    for (size_t i = start; i <= end; i++){
        data->OnAnimationFrame(i);

        frame->Render();
        frame->SwapBuffer();
        
        builder.Append(dir);
        builder.Append(i);
        builder.Append(".png");
        SaveImage(builder.ToString(), rect);
        builder.Clear();
    }
}

Mesh* Main::GetMesh(){
    if (!data->curObject)
        return NULL;
    return data->curObject->GetMesh();
}

int Main::MainEntry(int argc, char** argv){
    AudioUtils::InitOpenAL();

    IWindow* mainFrame = new SelectionWindow(new MainWindow());

    AppFrame* appFrame = new AppFrame("ModelView", mainFrame, 600, 600);

    DebugLog("Main Window Created");

    appFrame->EnableOpenGL();
    appFrame->EnableOpenGLEXT();

    DebugLog("OpenGL Enabled");
    DebugLog("OpenGL Version %s", glGetString(GL_VERSION));
    DebugLog("OpenGL Renderer %s", glGetString(GL_RENDERER));
    DebugLog("OpenGL Vendor %s", glGetString(GL_VENDOR));
    //DebugLog("OpenGL Extensions %s", glGetString(GL_EXTENSIONS));

    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glFontSize(12);
    glInitASCIIFont();

    // 放在这里是为了让OpenGL初始化
    mainFrame->OnCreate();

    DebugLog("OpenGL Use Encoding %s", "GB2312");

    appFrame->Show();
    while (appFrame->WaitHandleEvent()){
        if (appFrame->reqRender || appFrame->GetLastMessageType() != AppFrame::MESSAGE_TIMER){
            appFrame->reqRender = false;
            appFrame->Render();
            appFrame->SwapBuffer();
        }
    }

    int code = appFrame->GetExitCode();

    delete appFrame;

    AudioUtils::UninitOpenAL();

    return code;
}

Main* Main::inst = NULL;
GlobalData* Main::data = NULL;

int main(int argc, char** argv){
    Main::inst = new Main();
    int ret = Main::inst->MainEntry(argc, argv);
    delete Main::inst;
    return ret;
}