#include <main.h>

#include <res.h>
#include <io/File.h>
#include <util/AudioUtils.h>
#include <util/StringBuilder.h>
#include <util/os/Log.h>
#include <util/os/Font.h>
#include <util/os/Time.h>
#include <util/os/Thread.h>
#include <util/os/Shell.h>
#include <util/os/Resource.h>
#include <util/gl/GLUtils.h>
#include <editor/AllWindows.h>
#include <editor/gui/Container.h>
#include <editor/gui/Menu.h>
#include <editor/dialog/Tips.h>
#include <editor/object/AllObjects.h>
#include <editor/windows/AllWindows.h>

#include <util/os/System.h>

#include <fstream>

#include <windows.h>
#include <gl/gl.h>
#include <lib/glut/glut.h>

#include <lib/json/nlohmann/json.hpp>

namespace simple3deditor {

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
    case IDM_EXIT:{
        static const WString caption = Resource::GetWString(IDS_EXIT_CONFIRM_CAPTION);
        static const WString text = Resource::GetWString(IDS_EXIT_CONFIRM);
        if (ShellMsgBox(caption, text) == MSGBOX_YES)
            ::PostQuitMessage(0);
    }
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
    case IDM_SAVE_WORKSPACE:{
        static const WString filter = Resource::GetWString(IDS_WORKSPACE_FILTER);
        WString file = ShellFileSelectWindow(filter, FILESELECT_REQ_PATH, true);
        if (file.GetLength() == 0){
            DebugError("Stop Saving");
            break;
        }
        try {
            nlohmann::json o;
            std::ofstream f(file.GetString());
            AppFrame::GetLocalInst()->GetMainFrame()->Serialize(o["windows"]);
            Main::data->scene->Serialize(o["objects"]["scene"]);
            Main::data->screen->Serialize(o["objects"]["screen"]);
            // json文件的缩进大小
            f.width(2);
            f << o;
            f.close();
        }catch(std::exception e){
            DebugError("Exception [%s] At %s %s", e.what(), __FILE__, __LINE__);
        }
    }
        break;
    case IDM_LOAD_WORKSPACE:{
        static const WString filter = Resource::GetWString(IDS_WORKSPACE_FILTER);
        WString file = ShellFileSelectWindow(filter, FILESELECT_REQ_FILE | FILESELECT_REQ_PATH);
        if (file.GetLength() == 0){
            DebugError("Stop Loading");
            break;
        }
        try {
            std::ifstream f(file.GetString());
            nlohmann::json o = nlohmann::json::parse(f, nullptr, false, true);
            nlohmann::json sub;
            if (!o.is_discarded()){
                if ((sub = o["windows"]).is_object()){
                    AWindow* window = Main::data->ConstructWindow(sub);
                    if (window){
                        dynamic_cast<SelectionWindow*>(AppFrame::GetLocalInst()->GetMainFrame())
                            ->SetWindow(window);
                    } else {
                        DebugError("Workspace File [windows] Is Broken");
                    }
                }
                if ((sub = o["objects"]).is_object()){
                    AViewObject* scene = Main::data->ConstructObject(sub["scene"]);
                    AViewObject* screen = Main::data->ConstructObject(sub["screen"]);
                    if (scene == NULL || screen == NULL){
                        DebugError("Workspace File [objects] Is Broken");
                    }else{
                        Main::data->scene = scene;
                        Main::data->screen = screen;
                    }
                }
            }
            f.close();
        }catch(std::exception e){
            DebugError("Exception [%s] At %s %s", e.what(), __FILE__, __LINE__);
        }
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

// 设置信息
static nlohmann::json settings;

GlobalData::GlobalData(){
    // register windows
    RegisterWindow<AWindow>();
    RegisterWindow<AGUIWindow>();

    RegisterWindow<LRContainer>();
    RegisterWindow<UDContainer>();
    RegisterWindow<SelectionWindow>();

    RegisterWindow<MainWindow>();
    RegisterWindow<TreeWindow>();
    RegisterWindow<AnimationWindow>();
    RegisterWindow<AttributeWindow>();
    RegisterWindow<AudioPlayerWindow>();
    RegisterWindow<AudioCaptureWindow>();
    RegisterWindow<NodeMapWindow>();
    RegisterWindow<PaintWindow>();
    RegisterWindow<RenderWindow>();
    RegisterWindow<UVEditWindow>();

    // register objects
    RegisterObject<AViewObject>();
    RegisterObject<AGUIObject>();
    RegisterObject<AWindowObject>();

    RegisterObject<AudioListenerObject>();
    RegisterObject<AudioSourceObject>();
    RegisterObject<CameraObject>();
    RegisterObject<CubicBezierObject>();
    RegisterObject<GUIManagerObject>();
    RegisterObject<GUIMeshObject>();
    RegisterObject<MeshObject>();
    RegisterObject<PointLightObject>();
    RegisterObject<SquareBezierObject>();

    RegisterObject<AudioPlayerWindowObject>();
    RegisterObject<TreeWindowObject>();

    scene = new AViewObject(L"Scene");
    screen = new AViewObject(L"Screen");
}

GlobalData::~GlobalData(){
    if (scene) delete scene;
    if (screen) delete screen;
    Free(windowReg);
    Free(objectReg);
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

void GlobalData::OnTimer(int id){
    scene->OnTimer(id);
    screen->OnTimer(id);
}

void GlobalData::OnAnimationFrame(float frame){
    animFrame = frame;
    scene->OnAnimationFrame(frame);
    screen->OnAnimationFrame(frame);
}

void GlobalData::RegisterWindow(std::function<AWindow*()> factory, const char* id, const wchar_t* displayName){
    WindowInfo* info = new WindowInfo;
    info->factory = factory;
    info->id = id;
    info->displayName = displayName;
    windowReg.Add(info);
}

AWindow* GlobalData::ConstructWindow(const char* id){
    WindowInfo* info = NULL;
    windowReg.Foreach([&](WindowInfo* item){
        if (!info && item->id == id)
            info = item;
    });
    if (!info){
        DebugError("GlobalData::ConstructWindow Unrecognized Window ID %s", id);
        return NULL;
    }
    return info->factory();
}

AWindow* GlobalData::ConstructWindow(nlohmann::json& o){
    AWindow* window = ConstructWindow(o.value("id", std::string()).c_str());
    if (window)
        window->Deserialize(o);
    return window;
}

AViewObject* GlobalData::ConstructObject(const char* id){
    ObjectInfo* info = NULL;
    objectReg.Foreach([&](ObjectInfo* item){
        if (!info && item->id == id)
            info = item;
    });
    if (!info){
        DebugError("GlobalData::ConstructObject Unrecognized Object ID %s", id);
        return NULL;
    }
    return info->factory();
}

AViewObject* GlobalData::ConstructObject(nlohmann::json& o){
    AViewObject* object = ConstructObject(o.value("id", std::string()).c_str());
    if (object)
        object->Deserialize(o);
    return object;
}

void GlobalData::LoadSettings(const char* path){
    try {
        std::ifstream f(path);
        settings = nlohmann::json::parse(f, nullptr, false, true);
        if (settings.is_discarded())
            settings = nlohmann::json::object({});
        f.close();
    }catch(std::exception e){
        DebugError("Exception [%s] At %s %s", e.what(), __FILE__, __LINE__);
    }
}

void GlobalData::ApplySettings(){
    nlohmann::json sub;
    if (settings.contains("menu") && (sub = settings["menu"]).is_object()){
        Menu::WIDTH_PIXELS = sub.value("width", 250.0f);
        Menu::CORNER_PIXELS = sub.value("corner", 10.0f);
        Menu::LINE_PIXELS = sub.value("line height", 30.0f);
    }
    if (settings.contains("main window") && (sub = settings["main window"]).is_object()){
        Main::INIT_WINDOW_WIDTH = sub.value("width", 600);
        Main::INIT_WINDOW_HEIGHT = sub.value("height", 600);
    }
}

void GlobalData::SaveSettings(const char* path){
    try {
        std::ofstream f(path);
        // json文件的缩进大??
        f.width(2);
        f << settings;
        f.close();
    }catch(std::exception e){
        DebugError("Exception [%s] At %s %s", e.what(), __FILE__, __LINE__);
    }
}

uint Main::INIT_WINDOW_WIDTH = 600;
uint Main::INIT_WINDOW_HEIGHT = 600;

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

bool Main::AddObjectChild(AViewObject* parent, AViewObject* child){
    if (parent->HasAncestor(child))
        return false;
    if (child == data->scene || child == data->screen)
        return false;
    parent->AddChild(child);
    return true;
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

void Main::OnMouseWheel(int delta){
    data->curObject->OnMouseWheel(delta);
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

    DebugLog("Load Settings");
    data->LoadSettings("settings.json");
    data->ApplySettings();

    SelectionWindow* mainFrame = new SelectionWindow();

    AppFrame* appFrame = new AppFrame("ModelView", mainFrame, INIT_WINDOW_HEIGHT, INIT_WINDOW_WIDTH);

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

    glEnableClientState(GL_VERTEX_ARRAY);

    // 不知道为什么，arial.ttf最大值字符为0x6FF
    glFontSize(12);

    mainFrame->SetWindow(new MainWindow());

    DebugLog("OpenGL Use Encoding %s", "GB2312");

    appFrame->Show();

    localData->recTime = Time::GetTime();
    localData->deltaTime = 0.0167f; // 一??60FPS大???的估测值，作为??动时间即??

    while (appFrame->WaitHandleEvent()){
        appFrame->Render();
        appFrame->SwapBuffer();

        float time = Time::GetTime();
        localData->deltaTime = time - localData->recTime;
        localData->recTime = time;
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

}

int main(int argc, char** argv){
    return simple3deditor::Main::MainEntry(argc, argv);
}