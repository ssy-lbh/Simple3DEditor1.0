#ifndef __PAINT__
#define __PAINT__

#include "define.h"

#include "uimgr.h"
#include "menu.h"
#include "geodef.h"

class UVEditWindow final : public IWindow {
private:
    bool focus = false;

    Vector2 cursorPos = Vector2::zero;
    Vector2 cliSize;
    float aspect;

    IOperation* curOp = NULL;
    ITool* curTool;

    class MoveOperation final : public IOperation {
    private:
        struct MoveInfo {
            Vertex* vert;
            Vector2 uv;
        };

        Vector2 start;
        List<MoveInfo> moveInfo;
        bool x, y;
        UVEditWindow* main;

    public:
        MoveOperation(UVEditWindow* main);
        virtual ~MoveOperation() override;
        virtual void OnEnter() override;
        virtual void OnMove() override;
        virtual void OnCommand(int id) override;
        virtual void OnConfirm() override;
        virtual void OnUndo() override;
    };

    class EmptyTool final : public ITool {
    private:
        UVEditWindow* window;

    public:
        EmptyTool(UVEditWindow* window);
        ~EmptyTool() override;
        virtual void OnLeftDown() override;
    };

    class SelectTool final : public ITool {
    private:
        UVEditWindow* window;
        Vector2 start;
        Vector2 end;
        bool leftDown;
        
    public:
        SelectTool(UVEditWindow* window);
        virtual ~SelectTool() override;
        virtual void OnLeftDown() override;
        virtual void OnLeftUp() override;
        virtual void OnMove() override;
        virtual void OnRender() override;
    };

public:
    UVEditWindow();
    ~UVEditWindow();

    virtual bool IsFocus() override;
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
    virtual void OnDropFileW(const wchar_t* path) override;

    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

    void SetOperation(IOperation* op);
    void SetTool(ITool* tool);
};

class PaintWindow final : public IWindow {
private:
    bool focus = false;

    Vector2 cursorPos = Vector2::zero;
    Vector2 cliSize;
    float aspect;

    Menu* basicMenu;

    IOperation* curOp = NULL;
    ITool* brush = NULL;

    GLuint paintTex = 0;
    GLint width, height;

    class ClearBrush final : public ITool {
    private:
        PaintWindow* window;
        GLuint colorLoc;
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
        GLuint paintLoc, offsetLoc, positionLoc, radiusLoc, colorLoc;
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
};

#endif