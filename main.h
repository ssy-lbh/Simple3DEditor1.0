#ifndef __MAIN__
#define __MAIN__

#include "define.h"

#include <gl/gl.h>
#include <gl/glu.h>

#include "appframe.h"
#include "menu.h"
#include "container.h"
#include "colorboard.h"
#include "viewobject.h"

//TODO 补全撤销功能
interface IUndo : public Object {
public:
    virtual void Execute() = 0;
};

class MainWindow final : public IWindow {
private:
    bool focus = false;
    Vector2 cursorPos = Vector2::zero;
    Vector3 cursorDir = Vector3::zero;
    Vector2 cliSize, cliInvSize;
    float aspect;

    bool lightEnabled = false;

    Vector3 camLookat = Vector3::up;
    Quaternion camDir = Quaternion::one;
    float camDis = 5.0f;
    Vector3 camPos = Vector3::back * 5.0f + Vector3::up;
    Vector3 camRight = Vector3::right;
    Vector3 camUp = Vector3::up;
    Vector3 camForward = Vector3::forward;
    float camRange = 100.0f;

    Menu* basicMenu;

    UIManager* uiMgr;

    IOperation* curOp = NULL;

    ColorBoard* colorBoard = NULL;

    ITool* curTool = NULL;

    WString inputText;
    bool inputConfirm = false;

    class MoveButton final : public IButton {
    private:
        Vector2 center;
        float radius;
        Vector3 start;
        MainWindow* main;
        GLTexture2D* texture = NULL;

    public:
        MoveButton(Vector2 center, float radius, MainWindow* main);
        virtual ~MoveButton() override;
        virtual bool Trigger(Vector2 pos) override;
        virtual void Render() override;
        virtual void Click(Vector2 pos) override;
        virtual void Drag(Vector2 dir) override;
        virtual void ClickEnd() override;
    };

    class RotateButton final : public IButton {
    private:
        Vector2 center;
        float radius;
        Quaternion start;
        MainWindow* main;

    public:
        RotateButton(Vector2 center, float radius, MainWindow* main);
        virtual ~RotateButton() override;
        virtual bool Trigger(Vector2 pos) override;
        virtual void Render() override;
        virtual void Click(Vector2 pos) override;
        virtual void Drag(Vector2 dir) override;
        virtual void ClickEnd() override;
    };

    class MoveOperation final : public IOperation {
    private:
        struct MoveInfo {
            Vertex* vert;
            Vector3 pos;
        };

        Vector2 start;
        List<MoveInfo> moveInfo;
        bool x, y, z;
        MainWindow* main;

    public:
        MoveOperation(MainWindow* main);
        virtual ~MoveOperation() override;
        virtual void OnEnter() override;
        virtual void OnMove() override;
        virtual void OnCommand(int id) override;
        virtual void OnConfirm() override;
        virtual void OnUndo() override;
    };

    class ExcludeOperation final : public IOperation {
    private:
        struct MoveInfo {
            Vertex* vert;
            Vector3 pos;
        };

        Vector2 start;
        List<MoveInfo> moveInfo;
        bool x, y, z;
        MainWindow* main;

    public:
        ExcludeOperation(MainWindow* main);
        virtual ~ExcludeOperation() override;
        virtual void OnEnter() override;
        virtual void OnMove() override;
        virtual void OnCommand(int id) override;
        virtual void OnConfirm() override;
        virtual void OnUndo() override;
    };

    class RotateOperation final : public IOperation {
    private:
        struct RotateInfo {
            Vertex* vert;
            Vector3 pos;
        };

        enum RotateMode {
            MODE_CAMERA,
            MODE_X,
            MODE_Y,
            MODE_Z
        };

        Vector2 start;
        List<RotateInfo> rotateInfo;
        RotateMode mode;
        MainWindow* main;
        Vector3 center;
        Vector2 screenCenter;
        float dis;
        Quaternion rotate;

    public:
        RotateOperation(MainWindow* main);
        virtual ~RotateOperation() override;
        virtual void OnEnter() override;
        virtual void OnMove() override;
        virtual void OnCommand(int id) override;
        virtual void OnConfirm() override;
        virtual void OnUndo() override;
    };

    class SizeOperation final : public IOperation {
    private:
        struct SizeInfo {
            Vertex* vert;
            Vector3 pos;
        };

        Vector3 center;
        Vector2 start;
        List<SizeInfo> sizeInfo;
        bool x, y, z;
        MainWindow* main;
        Vector2 screenCenter;
        float startSize;
        float scale;

    public:
        SizeOperation(MainWindow* main);
        virtual ~SizeOperation() override;
        virtual void OnEnter() override;
        virtual void OnMove() override;
        virtual void OnCommand(int id) override;
        virtual void OnConfirm() override;
        virtual void OnUndo() override;
    };

    class EmptyTool final : public ITool {
    private:
        MainWindow* window;

    public:
        EmptyTool(MainWindow* window);
        ~EmptyTool() override;
        virtual void OnLeftDown() override;
    };

    class SelectTool final : public ITool {
    private:
        MainWindow* window;
        Vector2 start;
        Vector2 end;
        bool leftDown;
        
    public:
        SelectTool(MainWindow* window);
        virtual ~SelectTool() override;
        virtual void OnLeftDown() override;
        virtual void OnLeftUp() override;
        virtual void OnMove() override;
        virtual void OnRender() override;
    };

    class LightItem final : public IMenuItem {
    private:
        MainWindow* window;
    
    public:
        LightItem(MainWindow* window);
        virtual ~LightItem() override;

        virtual const wchar_t* GetName() override;

        virtual void OnClick() override;
    };

public:
    MainWindow();
    virtual ~MainWindow() override;

    void InitCamera();
    void InitLight0();

    void RenderModelView();
    void SetOperation(IOperation* op);
    void SetTool(ITool* tool);

    void UpdateWindowSize(int x, int y);
    void UpdateCursor(int x, int y);
    void UpdateLookAtLocation();
    void UpdateRotation();
    void UpdateDistance();

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnClose() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileW(const wchar_t* path) override;

    void OnInsSave();
    void OnInsLoad();
    void OnInsTopology();
    void OnInsSelectColor();

    void GetTextInput();
    void AddPoint();
    void DeletePoint();
    bool SaveMesh(Mesh* mesh);
    bool LoadMesh(Mesh* mesh);
    bool LoadMesh(Mesh* mesh, WString path);
    void AboutBox();
    Vector3 GetLookPosition(Vector3 pos);
    Vector2 GetScreenPosition(Vector3 pos);
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

    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

    void SetMenu(Menu* m);

    void OnLeftDown(int x, int y);
    void OnLeftUp(int x, int y);
    void OnRightDown(int x, int y);
    void OnRightUp(int x, int y);

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
    //TODO 做出一个树状图窗口，显示场景中各个对象
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
    static void SetWindowCursor(int id);
    static void SetWindowCursor(const char* res);
    static void SetMenu(Menu* m);
    static void SelectObject(AViewObject* o);

    static Mesh* GetMesh();

    int MainEntry(int argc, char** argv);
};

#endif