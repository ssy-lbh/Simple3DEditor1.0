#ifndef __EDITOR_MAINWINDOW__
#define __EDITOR_MAINWINDOW__

#include <define.h>

#include <util/math3d/LinearAlgebra.h>
#include <util/math3d/Camera.h>
#include <editor/main/Window.h>

namespace simple3deditor {

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

    class MoveOperation;
    class ExcludeOperation;
    class RotateOperation;
    class SizeOperation;
    class EmptyTool;
    class SelectTool;

    friend class MoveOperation;
    friend class ExcludeOperation;
    friend class RotateOperation;
    friend class SizeOperation;
    friend class EmptyTool;
    friend class SelectTool;

    class MoveButton;
    class RotateButton;

    friend class MoveButton;
    friend class RotateButton;

protected:
    // 覆盖了父类函数
    void UpdateCursor(int x, int y);

    void SetRotation(Quaternion rot);
    void SetDistance(float dis);
    Point2 GetScreenPosition(Point3 pos);
    void InitCamera();

public:
    static constexpr const char* WINDOW_ID = "lbh.main";
    static constexpr const wchar_t* WINDOW_DISPLAY_NAME = L"主窗口";

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
    virtual void OnDropFileW(const wchar_t* path, uint len) override;

    virtual void Serialize(IOutputStream& os) override;
    virtual void Deserialize(IInputStream& os) override;

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

}

#endif