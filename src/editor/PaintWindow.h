#ifndef __EDITOR_PAINTWINDOW__
#define __EDITOR_PAINTWINDOW__

#include <define.h>

#include <util/math3d/LinearAlgebra.h>
#include <editor/main/Tool.h>
#include <editor/main/Operation.h>
#include <editor/main/Window.h>

namespace simple3deditor {

class PaintWindow final : public AWindow {
private:
    Menu* basicMenu;

    IOperation* curOp = NULL;
    ITool* brush = NULL;

    GLRenderTexture2D* paintTex = NULL;

    class ClearBrush;
    class DefaultBrush; 

    friend class ClearBrush;
    friend class DefaultBrush;

protected:
    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    static constexpr const char* WINDOW_ID = "lbh.paint";
    static constexpr const wchar_t* WINDOW_DISPLAY_NAME = L"绘画窗口";

    PaintWindow();
    virtual ~PaintWindow();

    virtual void OnRender() override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMenuAccel(int id, bool accel) override;

    virtual void Serialize(IOutputStream& os) override;
    virtual void Deserialize(IInputStream& os) override;

    void SetOperation(IOperation* op);
    void SetBrush(ITool* tool);

    void CreateImage(int x, int y);
    bool InRect(int x, int y);
    int GetWidth();
    int GetHeight();
};

}

#endif