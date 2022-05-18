#ifndef __MAIN__
#define __MAIN__

#include <define.h>

#include <util/HashMap.h>
#include <util/List.h>
#include <util/os/AppFrame.h>
#include <util/math3d/LinearAlgebra.h>
#include <util/physics3d/engine/PhysicsCommon.h>

namespace simple3deditor {

//TODO 补全撤销功能
class IUndo : public Object {
public:
    virtual void Execute() = 0;
};

struct RenderOptions {
public:
    bool vertex = true;
    bool edge = true;
    bool face = true;
    bool light = false;
    bool editor = false;
    ObjectOperation objOp;
};

//TODO 可加入窗口内拖拽功能
class LocalData final : public Object {
public:
    Vector2 cursorPos;
    Vector2 cliSize;
    float aspect = 1.0f;

    Menu* menu = NULL;
    Vector2 menuPos;

    RenderOptions renderOptions;

    // 这两种对象在单个窗口中唯一
    AudioListenerObject* audioListener = NULL;
    CameraObject* camera = NULL;

    // 外部程序不应修改
    float recTime;
    float deltaTime;

    // 无延迟渲染
    bool reqRender = false;

    LocalData();
    ~LocalData();

    static LocalData* GetLocalInst();

    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

    void SetMenu(Menu* m);

    void OnLeftDown(int x, int y);
    void OnLeftUp(int x, int y);
    void OnRightDown(int x, int y);
    void OnRightUp(int x, int y);
    void OnMenuAccel(int id, bool accel);

    void Render();

    void CreateAudioListener();
    void CreateCamera();
    void DestoryAudioListener();
    void DestoryCamera();
};

enum class SelectionType {
    SELECT_OBJECT,
    SELECT_VERTICES,
    SELECT_EDGES,
    SELECT_FACES
};

struct WindowInfo {
    std::function<AWindow*()> factory;
    WString displayName;
};

//TODO 可加入全局拖拽功能，在窗口之间传送数据
class GlobalData final : public Object {
public:
    // 总场景对象
    AViewObject* scene = NULL;
    AViewObject* screen = NULL;

    // 当前选中的对象
    AViewObject* curObject = NULL;

    // 选择部分
    //TODO 以后将选择部分信息移入线程本地
    SelectionType selType = SelectionType::SELECT_OBJECT;

    List<AViewObject*> selObjects;
    List<Vertex*> selPoints;
    List<Edge*> selEdges;
    List<Face*> selFaces;

    float animFrame = 0.0f;

    reactphysics3d::PhysicsCommon* physicsCommon = NULL;
    reactphysics3d::PhysicsWorld* physicsWorld = NULL;

    GlobalData();
    ~GlobalData();

    void SelectObject(AViewObject* o);
    void SelectType(SelectionType type);

    void OnTimer(int id);

    void OnAnimationFrame(float frame);

    HashMapA<WindowInfo*> windowReg;

    // id用于标记文件等数据中的类型信息，对于每一类的类应唯一
    // 注册的窗口类有以下要求
    // 1. 继承自editor/main/Window.h中的AWindow类
    // 2. 内部定义static constexpr const char* WINDOW_ID作为全局窗口唯一标识符，注意唯一
    // 3. 内部定义static constexpr const wchar_t* WINDOW_DISPLAY_NAME作为窗口显示名称，内容不作要求
    // 4. 如果窗口不能给用户自由选择和使用(比如容器)，可将WINDOW_DISPLAY_NAME设为L""
    template <typename T>
    void RegisterWindow(){
        WindowInfo* info = new WindowInfo;
        info->factory = []{ return new T(); };
        info->displayName = T::WINDOW_DISPLAY_NAME;
        windowReg.Set(T::WINDOW_ID, info);
    }

    AWindow* ConstructWindow(const String& id);
    AWindow* ConstructWindow(IInputStream& is);
};

class Main final : public Object {
public:
    static GlobalData* data;

    static void SetCursor(int id);
    static void SetCursor(const char* res);
    static void SetMenu(Menu* m);
    static void SelectObject(AViewObject* o);
    static void SelectType(SelectionType type);
    static void AddObject(AViewObject* o);
    static void AddObjectToScreen(AViewObject* o);
    static bool AddObjectChild(AViewObject* parent, AViewObject* child);
    static void DeleteObject(AViewObject* o);
    static void OnAnimationFrame(float frame);
    static void SaveImage(String file, Rect rect);
    static void RenderAnimation(String dir, size_t start, size_t end, Rect rect);
    // 请求无延迟渲染
    static void RequestRender();

    static void OnMouseMove(Point3 ori, Vector3 dir);
    static void OnLeftDown(Point3 ori, Vector3 dir);
    static void OnLeftUp(Point3 ori, Vector3 dir);
    static void OnRightDown(Point3 ori, Vector3 dir);
    static void OnRightUp(Point3 ori, Vector3 dir);
    static void OnMouseWheel(int delta);

    static void RenderScene();
    static void RenderScreen();

    static Mesh* GetMesh();
    static Mesh* GetMesh(AViewObject* o);

    static int MainEntry(int argc, char** argv);
    static void Init();
    static void Uninit();
};

}

#endif