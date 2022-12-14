#ifndef __EDITOR_UVEDITWINDOW__
#define __EDITOR_UVEDITWINDOW__

#include <define.h>

#include <editor/main/Window.h>

namespace simple3deditor {

class UVEditWindow final : public AWindow {
private:
    IOperation* curOp = NULL;
    ITool* curTool = NULL;

    class MoveOperation;
    class EmptyTool;
    class SelectTool;

    friend class MoveOperation;
    friend class EmptyTool;
    friend class SelectTool;

protected:
    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    WINDOW_INFO_DEF();

    UVEditWindow();
    ~UVEditWindow();

    virtual void OnRender() override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileW(const wchar_t* path, uint len) override;

    virtual void Serialize(nlohmann::json& o) override;
    virtual void Deserialize(nlohmann::json& o) override;

    void SetOperation(IOperation* op);
    void SetTool(ITool* tool);
};

}

#endif