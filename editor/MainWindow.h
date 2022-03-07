#ifndef __EDITOR_MAINWINDOW__
#define __EDITOR_MAINWINDOW__

#include <define.h>

#include <editor/gui/UIManager.h>
#include <editor/gui/Menu.h>

enum class ObjectOperation {
    MOVE,
    ROTATE,
    SCALE
};

class MainWindow final : public IWindow {
private:
    bool focus = false;
    Point2 cursorPos = Point2::zero;
    Vector3 cursorDir = Vector3::zero;
    Vector2 cliSize, cliInvSize;
    float aspect;

    bool lightEnabled = false;
    bool audioControl = true;
    bool dopplerEffect = true;

    Point3 camLookat = Point3(0.0f, 0.0f, 1.0f);
    Quaternion camDir = Quaternion::one;
    float camDis = 5.0f;
    Point3 camPos = Point3(0.0f, -5.0f, 1.0f);
    Vector3 camRight = Vector3::right;
    Vector3 camUp = Vector3::up;
    Vector3 camForward = Vector3::forward;
    float camRange = 100.0f;

    Menu* basicMenu;
    Menu* insertMenu;

    UIManager* uiMgr;

    IOperation* curOp = NULL;

    ITool* curTool = NULL;

    GLSkyBox* skyBox = NULL;

    ObjectOperation objOp = ObjectOperation::MOVE;

    class MoveButton final : public IButton {
    private:
        Point2 center;
        float radius;
        Point3 start;
        MainWindow* main;
        GLTexture2D* texture = NULL;

    public:
        MoveButton(Vector2 center, float radius, MainWindow* main);
        virtual ~MoveButton() override;
        virtual bool Trigger(Vector2 pos) override;
        virtual void Render() override;
        virtual void Click(Vector2 pos) override;
        virtual void Drag(Vector2 dir) override;
        virtual void ClickEnd(Vector2 pos, IButton* end) override;
    };

    class RotateButton final : public IButton {
    private:
        Point2 center;
        float radius;
        Quaternion start;
        Vector3 up;
        Vector3 right;
        MainWindow* main;

    public:
        RotateButton(Vector2 center, float radius, MainWindow* main);
        virtual ~RotateButton() override;
        virtual bool Trigger(Vector2 pos) override;
        virtual void Render() override;
        virtual void Click(Vector2 pos) override;
        virtual void Drag(Vector2 dir) override;
        virtual void ClickEnd(Vector2 pos, IButton* end) override;
    };

    class MoveOperation final : public IOperation {
    private:
        struct MoveInfo {
            union {
                AViewObject* obj;
                Vertex* vert;
                Edge* e;
                Face* f;
            };
            Vector3 pos;
        };

        Point2 start;
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
            Point3 pos;
        };

        Point2 start;
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
            union {
                AViewObject* obj;
                Vertex* vert;
                Edge* e;
                Face* f;
            };
            Point3 pos;
            Quaternion rot;
        };

        enum RotateMode {
            MODE_CAMERA,
            MODE_X,
            MODE_Y,
            MODE_Z
        };

        Point2 start;
        List<RotateInfo> rotateInfo;
        RotateMode mode;
        MainWindow* main;
        Point3 center;
        Point2 screenCenter;
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
            union {
                AViewObject* obj;
                Vertex* vert;
                Edge* e;
                Face* f;
            };
            Vector3 vec;
        };

        Point3 center;
        Point2 start;
        List<SizeInfo> sizeInfo;
        bool x, y, z;
        MainWindow* main;
        Point2 screenCenter;
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
        Point2 start;
        Point2 end;
        bool leftDown;
        
    public:
        SelectTool(MainWindow* window);
        virtual ~SelectTool() override;
        virtual void OnLeftDown() override;
        virtual void OnLeftUp() override;
        virtual void OnMove() override;
        virtual void OnRender() override;
    };

public:
    MainWindow();
    virtual ~MainWindow() override;

    void InitCamera();

    void RenderModelView();
    void SetOperation(IOperation* op);
    void SetTool(ITool* tool);

    void UpdateWindowSize(int x, int y);
    void UpdateCursor(int x, int y);
    void UpdateLookAtLocation();
    void UpdateRotation();
    void UpdateDistance();

    virtual bool IsFocus() override;
    virtual void OnCreate() override;
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

    void AddPoint();
    void DeletePoint();
    bool SaveMesh(Mesh* mesh);
    bool LoadMesh(AViewObject* obj);
    bool LoadMesh(AViewObject* obj, WString path);
    Point3 GetLookPosition(Point3 pos);
    Point2 GetScreenPosition(Point3 pos);
};

#endif