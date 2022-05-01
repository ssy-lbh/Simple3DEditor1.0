#ifndef __EDITOR_MAINWINDOW__
#define __EDITOR_MAINWINDOW__

#include <define.h>

#include <util/math3d/LinearAlgebra.h>
#include <util/math3d/Camera.h>
#include <editor/main/Window.h>
#include <editor/main/Tool.h>
#include <editor/main/Operation.h>
#include <editor/gui/Menu.h>

enum class ObjectOperation {
    MOVE,
    ROTATE,
    SCALE
};

class MainWindow final : public AWindow, public CCamera {
private:
    Vector3 cursorDir;

    bool lightEnabled = false;
    bool audioControl = true;
    bool dopplerEffect = true;

    float camRange = 100.0f;

    GUIManagerObject* guiMgr;

    Menu* basicMenu;
    Menu* insertMenu;

    IOperation* curOp = NULL;

    ITool* curTool = NULL;

    GLSkyBox* skyBox = NULL;

    ObjectOperation objOp = ObjectOperation::MOVE;

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

protected:
    // 覆盖了父类函数
    void UpdateCursor(int x, int y);

    void SetRotation(Quaternion rot);
    void SetDistance(float dis);
    Point2 GetScreenPosition(Point3 pos);
    void InitCamera();

public:
    MainWindow();
    virtual ~MainWindow() override;

    void RenderModelView();
    void SetOperation(IOperation* op);
    void SetTool(ITool* tool);

    virtual void OnRender() override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMouseWheel(int delta) override;
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
};

#endif