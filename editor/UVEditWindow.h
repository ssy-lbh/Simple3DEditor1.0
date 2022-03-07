#ifndef __EDITOR_UVEDITWINDOW__
#define __EDITOR_UVEDITWINDOW__

#include <define.h>

#include <editor/gui/UIManager.h>

class UVEditWindow final : public IWindow {
private:
    bool focus = false;

    Vector2 cursorPos = Vector2::zero;
    Vector2 cliSize;
    float aspect;

    IOperation* curOp = NULL;
    ITool* curTool = NULL;

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

#endif