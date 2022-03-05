#ifndef __EDITOR_PAINTWINDOW__
#define __EDITOR_PAINTWINDOW__

#include <define.h>

#include <editor/gui/UIManager.h>
#include <utils/math3d/LinearAlgebra.h>

class PaintWindow final : public IWindow {
private:
    bool focus = false;

    Vector2 cursorPos = Vector2::zero;
    Vector2 cliSize;
    float aspect;

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
        uint paintLoc, offsetLoc, positionLoc, radiusLoc, colorLoc;
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

public:
    PaintWindow();
    virtual ~PaintWindow();

    virtual bool IsFocus() override;
    virtual void OnCreate() override;
    virtual void OnRender() override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMenuAccel(int id, bool accel) override;

    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

    void SetOperation(IOperation* op);
    void SetBrush(ITool* tool);

    void CreateImage(int x, int y);
    bool InRect(int x, int y);
    int GetWidth();
    int GetHeight();
};

#endif