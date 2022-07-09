#ifndef __EDITOR_MAIN_WINDOW__
#define __EDITOR_MAIN_WINDOW__

#include <define.h>

#include <io/Serializable.h>
#include <util/math3d/LinearAlgebra.h>

namespace simple3deditor {

class AWindow : public Object, public IMemorable {
protected:
    bool focus = false;

    Point2 cursorCoord;
    Point2 cursorPos = Point2::zero;
    Vector2 cliSize;
    Vector2 cliInvSize;
    float aspect;

    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    static constexpr const char* WINDOW_ID = "lbh.base.wnd";
    static constexpr const wchar_t* WINDOW_DISPLAY_NAME = L"";

    AWindow();
    virtual ~AWindow();

    virtual bool IsFocus();
    // 基于OpenGL的渲染，如果渲染全过程已做好抽象可换用API
    virtual void OnRender();
    virtual void OnTimer(int id);
    virtual void OnChar(char c);
    virtual void OnUnichar(wchar_t c);
    virtual void OnResize(int x, int y);
    virtual void OnMouseMove(int x, int y);
    virtual void OnLeftDown(int x, int y);
    virtual void OnLeftUp(int x, int y);
    virtual void OnRightDown(int x, int y);
    virtual void OnRightUp(int x, int y);
    virtual void OnFocus();
    virtual void OnKillFocus();
    virtual void OnMouseWheel(int delta);
    virtual void OnMenuAccel(int id, bool accel);
    virtual void OnDropFileA(const char* path, uint len);
    virtual void OnDropFileW(const wchar_t* path, uint len);

    // 窗口序列化时应先os.WriteWithLen(WINDOW_ID)，这样才能在反序列化时识别窗口
    virtual void Serialize(json& o) override;
    // 除了WINDOW_ID，其他数据按顺序依次读取就行
    virtual void Deserialize(json& o) override;
};

class AGUIWindow : public AWindow {
protected:
    GUIManagerObject* guiMgr;

public:
    static constexpr const char* WINDOW_ID = "lbh.base.guiwnd";
    static constexpr const wchar_t* WINDOW_DISPLAY_NAME = L"";

    AGUIWindow();
    virtual ~AGUIWindow() override;

    virtual void OnRender() override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;

    virtual void Serialize(json& o) override;
    using AWindow::Deserialize;
};

}

#endif