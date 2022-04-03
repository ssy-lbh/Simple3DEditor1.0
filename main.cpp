#include <main.h>

#include <lib/opengl/gl/gl.h>
#include <lib/glut/glut.h>

#include <res.h>
#include <editor/gui/Container.h>
#include <editor/gui/Menu.h>
#include <editor/MainWindow.h>
#include <editor/dialog/Tips.h>
#include <utils/AudioUtils.h>
#include <utils/StringBuilder.h>
#include <utils/os/Log.h>
#include <utils/os/Font.h>
#include <utils/os/Time.h>
#include <utils/os/Thread.h>
#include <utils/os/Shell.h>
#include <utils/os/Resource.h>
#include <utils/gl/GLUtils.h>
#include <editor/main/ViewObject.h>
#include <editor/object/MeshObject.h>
#include <editor/object/GUIMeshObject.h>
#include <editor/object/CameraObject.h>
#include <editor/object/AudioListenerObject.h>

LocalData::LocalData(){
    renderOptions.objOp = ObjectOperation::MOVE;
}

LocalData::~LocalData(){}

LocalData* LocalData::GetLocalInst(){
    return static_cast<LocalData*>(ThreadLocal::Get(THREAD_LOCAL_LOCALDATA));
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
        static const WString caption = Resource::GetWString(IDS_EXIT_CONFIRM_CAPTION);
        static const WString text = Resource::GetWString(IDS_EXIT_CONFIRM);
        if (ShellMsgBox(caption, text) == MSGBOX_YES)
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
        menu->Render(menuPos);
}

void LocalData::CreateAudioListener(){
    if (audioListener){
        DebugError("LocalData::CreateAudioListener Trying To Create Two AudioListenerObject");
        return;
    }
    audioListener = new AudioListenerObject();
    Main::data->scene->AddChild(audioListener);
}

void LocalData::CreateCamera(){
    if (camera){
        DebugError("LocalData::CreateCamera Trying To Create Two CameraObject");
        return;
    }
    camera = new CameraObject();
    Main::data->scene->AddChild(camera);
}

void LocalData::DestoryAudioListener(){
    if (!audioListener){
        DebugError("LocalData::DestoryAudioListener No AudioListenerObject Created");
        return;
    }
    delete audioListener;
    audioListener = NULL;
}

void LocalData::DestoryCamera(){
    if (!camera){
        DebugError("LocalData::DestoryCamera No CameraObject Created");
        return;
    }
    delete camera;
    camera = NULL;
}

GlobalData::GlobalData(){
    scene = new AViewObject(L"Scene");
    screen = new AViewObject(L"Screen");
}

GlobalData::~GlobalData(){
    if (scene) delete scene;
    if (screen) delete screen;
}

void GlobalData::SelectObject(AViewObject* o){
    if (!o){
        DebugError("Warning: GlobalData::SelectObject Object Is NULL");
        return;
    }
    curObject = o;
    switch (selType){
    case SelectionType::SELECT_OBJECT:
        selObjects.Clear();
        break;
    case SelectionType::SELECT_VERTICES:
        selPoints.Clear();
        break;
    case SelectionType::SELECT_EDGES:
        selEdges.Clear();
        break;
    case SelectionType::SELECT_FACES:
        selFaces.Clear();
        break;
    }
}

void GlobalData::SelectType(SelectionType type){
    if (selType == type)
        return;
    switch (selType){
    case SelectionType::SELECT_OBJECT:
        selObjects.Clear();
        break;
    case SelectionType::SELECT_VERTICES:
        selPoints.Clear();
        break;
    case SelectionType::SELECT_EDGES:
        selEdges.Clear();
        break;
    case SelectionType::SELECT_FACES:
        selFaces.Clear();
        break;
    }
    selType = type;
}

void GlobalData::OnCreate(){
    scene->OnCreate();
    screen->OnCreate();
}

void GlobalData::OnClose(){
    scene->OnClose();
    screen->OnClose();
}

void GlobalData::OnTimer(int id){
    scene->OnTimer(id);
    screen->OnTimer(id);
}

void GlobalData::OnAnimationFrame(float frame){
    animFrame = frame;
    scene->OnAnimationFrame(frame);
    screen->OnAnimationFrame(frame);
}

#ifdef PLATFORM_WINDOWS
#include <utils/os/System.h>

void Main::SetCursor(int id){
    AppFrame* frame = AppFrame::GetLocalInst();
    ::SetCursor(LoadCursorA(GetModule(), MAKEINTRESOURCEA(id)));
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

void Main::SelectType(SelectionType type){
    data->SelectType(type);
}

void Main::AddObject(AViewObject* o){
    data->scene->AddChild(o);
}

void Main::AddObjectToScreen(AViewObject* o){
    data->screen->AddChild(o);
}

void Main::DeleteObject(AViewObject* o){
    if (o == data->scene){
        DebugError("Main::DeleteObject Trying To Delete Scene Object");
        return;
    }
    if (o == data->screen){
        DebugError("Main::DeleteObject Trying To Delete Screen Object");
        return;
    }
    delete o;
    if (o == data->curObject)
        data->SelectObject(data->scene);
}

void Main::OnAnimationFrame(float frame){
    data->OnAnimationFrame(frame);
}

void Main::SaveImage(String file, Rect rect){
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

void Main::RenderAnimation(String dir, size_t start, size_t end, Rect rect){
    AppFrame* frame = AppFrame::GetLocalInst();
    StringBuilderA builder;

    for (size_t i = start; i <= end; i++){
        data->OnAnimationFrame(i);

        frame->Render();
        frame->SwapBuffer();
        
        builder += dir;
        builder += i;
        builder += ".png";
        SaveImage(builder.ToString(), rect);
        builder.Clear();
    }
}

void Main::RequestRender(){
    LocalData::GetLocalInst()->reqRender = true;
}

void Main::OnMouseMove(Point3 ori, Vector3 dir){
    data->curObject->OnMouseMove(ori, dir);
}

void Main::OnLeftDown(Point3 ori, Vector3 dir){
    data->curObject->OnLeftDown(ori, dir);
}

void Main::OnLeftUp(Point3 ori, Vector3 dir){
    data->curObject->OnLeftUp(ori, dir);
}

void Main::OnRightDown(Point3 ori, Vector3 dir){
    data->curObject->OnRightDown(ori, dir);
}

void Main::OnRightUp(Point3 ori, Vector3 dir){
    data->curObject->OnRightUp(ori, dir);
}

void Main::RenderScene(){
    data->scene->OnChainRender();
}

void Main::RenderScreen(){
    data->screen->OnChainRender();
}

Mesh* Main::GetMesh(){
    return GetMesh(data->curObject);
}

Mesh* Main::GetMesh(AViewObject* o){
    if (!o)
        return NULL;
    if (o->GetType() == ViewObjectType::OBJECT_MESH){
        return (dynamic_cast<MeshObject*>(o))->GetMesh();
    }
    if (o->GetType() == ViewObjectType::OBJECT_GUI_MESH){
        return (dynamic_cast<GUIMeshObject*>(o))->GetMesh();
    }
    return NULL;
}

int Main::MainEntry(int argc, char** argv){
    Init();
    AudioUtils::InitOpenAL();

    AWindow* mainFrame = new SelectionWindow(new MainWindow());

    AppFrame* appFrame = new AppFrame("ModelView", mainFrame, 600, 600);

    LocalData* localData = LocalData::GetLocalInst();

    DebugLog("Main Window Created");

    appFrame->EnableOpenGL();
    glutInit(&argc, argv);

    DebugLog("OpenGL Enabled");
    DebugLog("OpenGL Version %s", glGetString(GL_VERSION));
    DebugLog("OpenGL Renderer %s", glGetString(GL_RENDERER));
    DebugLog("OpenGL Vendor %s", glGetString(GL_VENDOR));
    //DebugLog("OpenGL Extensions %s", glGetString(GL_EXTENSIONS));

    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // 不知道为什么，arial.ttf最大值字符为0x6FF
    glFontSize(12);

    // 放在这里是为了让OpenGL初始化
    mainFrame->OnCreate();
    data->OnCreate();

    DebugLog("OpenGL Use Encoding %s", "GB2312");

    appFrame->Show();

    localData->recTime = Time::GetTime();
    localData->deltaTime = 0.0167f; // 一个60FPS大概的估测值，作为启动时间即可

    while (appFrame->HandleEvents()){
        appFrame->Render();
        appFrame->SwapBuffer();

        float time = Time::GetTime();
        localData->deltaTime = time - localData->recTime;
        localData->recTime = time;
        // 不知道为什么，我的电脑开不了垂直同步，只能出此下策了
        if (localData->reqRender){
            localData->reqRender = false;
        }else{
            Sleep(16 - localData->deltaTime);
        }
    }

    int code = appFrame->GetExitCode();

    delete appFrame;

    AudioUtils::UninitOpenAL();
    Uninit();

    return code;
}

void Main::Init(){
    MeshObject* mesh;

    data = new GlobalData();
    mesh = new MeshObject();
    
    AddObject(mesh);
    SelectObject(mesh);
}

void Main::Uninit(){
    if (data) delete data;
}

GlobalData* Main::data = NULL;

int main(int argc, char** argv){
    return Main::MainEntry(argc, argv);
}