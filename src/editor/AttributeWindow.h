#ifndef __EDITOR_ATTRIBUTEWINDOW__
#define __EDITOR_ATTRIBUTEWINDOW__

#include <define.h>

#include <editor/main/Window.h>

namespace simple3deditor {

//TODO 等待开发，显示对象属性
class AttributeWindow : public AWindow {
private:
    GUIManagerObject* guiMgr;

protected:
    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    WINDOW_INFO_DEF();

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

    virtual void Serialize(nlohmann::json& o) override;
    virtual void Deserialize(nlohmann::json& o) override;
};

}

#endif