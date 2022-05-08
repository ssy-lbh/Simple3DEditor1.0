#ifndef __EDITOR_ATTRIBUTEWINDOW__
#define __EDITOR_ATTRIBUTEWINDOW__

#include <define.h>

#include <editor/main/Window.h>

//TODO 等待开发，显示对象属性
class AttributeWindow : public AWindow {
private:
    GUIManagerObject* guiMgr;

protected:
    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    static constexpr const char* WINDOW_ID = "lbh.attr";
    static constexpr const wchar_t* WINDOW_DISPLAY_NAME = L"属性窗口(未完成)";

    AttributeWindow();
    virtual ~AttributeWindow() override;

    virtual void OnRender() override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;

    virtual void Serialize(IOutputStream& os) override;
    virtual void Deserialize(IInputStream& os) override;
};

#endif