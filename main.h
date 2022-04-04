#ifndef __MAIN__
#define __MAIN__

#include <define.h>

#include <utils/List.h>
#include <utils/os/AppFrame.h>
#include <utils/math3d/LinearAlgebra.h>

//TODO 补全撤销功能
interface IUndo : public Object {
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

    GlobalData();
    ~GlobalData();

    void SelectObject(AViewObject* o);
    void SelectType(SelectionType type);

    void OnCreate();
    void OnClose();
    void OnTimer(int id);

    void OnAnimationFrame(float frame);
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

    static void RenderScene();
    static void RenderScreen();

    static Mesh* GetMesh();
    static Mesh* GetMesh(AViewObject* o);

    static int MainEntry(int argc, char** argv);
    static void Init();
    static void Uninit();
};

#endif