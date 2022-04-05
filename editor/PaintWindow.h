#ifndef __EDITOR_PAINTWINDOW__
#define __EDITOR_PAINTWINDOW__

#include <define.h>

#include <utils/math3d/LinearAlgebra.h>
#include <editor/main/Tool.h>
#include <editor/main/Operation.h>
#include <editor/main/Window.h>


class PaintWindow final : public AWindow {
private:
    Menu* basicMenu;

    IOperation* curOp = NULL;
    ITool* brush = NULL;

    GLRenderTexture2D* paintTex = NULL;

    class ClearBrush final : public ITool {
    private:
        PaintWindow* window;
        uint colorLoc;
        Vector3 color = Vector3::one;

        GLComputeProgram* prog = NULL;
        bool err = false;

    public:
        ClearBrush(PaintWindow* window);
        virtual ~ClearBrush() override;

        virtual void OnLeftDown() override;
        virtual void OnCommand(int id) override;
    };

    class DefaultBrush final : public ITool {
    private:
        PaintWindow* window;
        bool draw = false;

        // Uniform位置
        uint paintLoc;
        uint offsetLoc;
        uint positionLoc;
        uint radiusLoc;
        uint colorLoc;

        Vector3 color = Vector3::one;

        GLComputeProgram* prog = NULL;
        bool err = false;

    public:
        DefaultBrush(PaintWindow* window);
        DefaultBrush(PaintWindow* window, int shaderId);
        virtual ~DefaultBrush() override;

        virtual void OnLeftDown() override;
        virtual void OnLeftUp() override;
        virtual void OnMove() override;
        virtual void OnCommand(int id) override;

        void Draw();
    };

protected:
    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    PaintWindow();
    virtual ~PaintWindow();

    virtual void OnCreate() override;
    virtual void OnRender() override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMenuAccel(int id, bool accel) override;

    void SetOperation(IOperation* op);
    void SetBrush(ITool* tool);

    void CreateImage(int x, int y);
    bool InRect(int x, int y);
    int GetWidth();
    int GetHeight();
};

#endif