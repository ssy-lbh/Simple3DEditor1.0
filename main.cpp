#include <main.h>

#include <res.h>
#include <editor/gui/Container.h>
#include <editor/gui/Menu.h>
#include <editor/MainWindow.h>
#include <utils/AudioUtils.h>
#include <utils/os/Log.h>
#include <utils/os/Font.h>
#include <utils/os/Thread.h>
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
        return;
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

void LocalData::Render(){
    if (menu)
        menu->Render(menuPos.x, menuPos.y);
}

GlobalData::GlobalData(){
    curObject = new MeshObject();
    scene = new AViewObject(L"Scene");
    scene->AddChild(curObject);
}

GlobalData::~GlobalData(){
    if (scene) delete scene;
}

void GlobalData::SelectObject(AViewObject* o){
    curObject = o;
    selObjects.Clear();
    selPoints.Clear();
    selEdges.Clear();
    selFaces.Clear();
}

Main::Main(){
    data = new GlobalData();
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

Mesh* Main::GetMesh(){
    if (!data->curObject)
        return NULL;
    return data->curObject->GetMesh();
}

int Main::MainEntry(int argc, char** argv){
    mainFrame = new SelectionWindow(new MainWindow());

    appFrame = new AppFrame("ModelView", mainFrame, 750, 1250);

    DebugLog("Main Window Created");

    appFrame->EnableOpenGL();
    appFrame->EnableOpenGLEXT();

    DebugLog("OpenGL Enabled");
    DebugLog("OpenGL Version %s", glGetString(GL_VERSION));
    DebugLog("OpenGL Renderer %s", glGetString(GL_RENDERER));
    DebugLog("OpenGL Vendor %s", glGetString(GL_VENDOR));

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