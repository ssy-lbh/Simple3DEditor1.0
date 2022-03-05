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
    SelectionType selType = SELECT_VERTICES;

    List<AViewObject*> selObjects;
    List<Vertex*> selPoints;
    List<Edge*> selEdges;
    List<Face*> selFaces;

    GlobalData();
    ~GlobalData();

    void SelectObject(AViewObject* o);

    void OnAnimationFrame(float frame);
};

class Main final : public Object {
public:
    static Main* inst;
    static GlobalData* data;

    Main();
    ~Main();

    static void RequestRender();
    static void SetCursor(int id);
    static void SetCursor(const char* res);
    static void SetMenu(Menu* m);
    static void SelectObject(AViewObject* o);
    static AViewObject* AddObject(AViewObject* o);
    static void OnAnimationFrame(float frame);

    static Mesh* GetMesh();

    int MainEntry(int argc, char** argv);
};

#endif