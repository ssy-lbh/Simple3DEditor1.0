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

//TODO 可加入窗口内拖拽功能
class LocalData final : public Object {
public:
    Vector2 cursorPos;
    Vector2 cliSize;
    float aspect;

    Menu* menu = NULL;
    Vector2 menuPos;

    // 这两种对象在单个窗口中唯一
    AudioListenerObject* audioListener = NULL;
    CameraObject* camera = NULL;

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

//TODO 可加入全局拖拽功能，在窗口之间传送数据
class GlobalData final : public Object {
public:
    enum SelectionType {
        SELECT_OBJECT,
        SELECT_VERTICES,
        SELECT_EDGES,
        SELECT_FACES
    };

    Vector3 audioPos = Vector3::zero;

    float animFrame = 0.0f;

    // 总场景对象
    AViewObject* scene = NULL;

    // 当前选中的对象
    AViewObject* curObject = NULL;

    // 选择部分
    //TODO 以后将选择部分信息移入线程本地
    SelectionType selType = SELECT_OBJECT;

    List<AViewObject*> selObjects;
    List<Vertex*> selPoints;
    List<Edge*> selEdges;
    List<Face*> selFaces;

    GlobalData();
    ~GlobalData();

    void SelectObject(AViewObject* o);
    void SelectType(GlobalData::SelectionType type);

    void OnAnimationFrame(float frame);
};

class Main final : public Object {
public:
    static GlobalData* data;

    static void RequestRender();
    static void SetCursor(int id);
    static void SetCursor(const char* res);
    static void SetMenu(Menu* m);
    static void SelectObject(AViewObject* o);
    static void SelectType(GlobalData::SelectionType type);
    static void AddObject(AViewObject* o);
    static void DeleteObject(AViewObject* o);
    static void OnAnimationFrame(float frame);
    static void SaveImage(String file, GLRect rect);
    static void RenderAnimation(String dir, size_t start, size_t end, GLRect rect);

    static Mesh* GetMesh();

    static int MainEntry(int argc, char** argv);
    static void Init();
    static void Uninit();
};

#endif