#ifndef __MAIN__
#define __MAIN__

#include <define.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include <utils/List.h>
#include <utils/os/AppFrame.h>
#include <utils/math3d/LinearAlgebra.h>

//TODO 补全撤销功能
interface IUndo : public Object {
public:
    virtual void Execute() = 0;
};

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
};

class Main final : public Object {
public:
    static Main* inst;
    static GlobalData* data;

    AppFrame* appFrame = NULL;

    // 测试过面向对象的结果，成功实现多屏
    IWindow* mainFrame = NULL;

    Main();
    ~Main();

    static void RequestRender();
    static void SetCursor(int id);
    static void SetCursor(const char* res);
    static void SetMenu(Menu* m);
    static void SelectObject(AViewObject* o);

    static Mesh* GetMesh();

    int MainEntry(int argc, char** argv);
};

#endif