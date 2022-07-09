#ifndef __EDITOR_TREEWINDOW__
#define __EDITOR_TREEWINDOW__

#include <define.h>

#include <util/List.h>
#include <editor/main/Window.h>

namespace simple3deditor {

class TreeWindow final : public AWindow {
private:
    List<AViewObject*> objectList;
    int depth;

    Menu* basicMenu;

    AViewObject* dragObject = NULL;
    AViewObject* selObject = NULL;

    void AddObject(AViewObject* o);

protected:
    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    static constexpr const char* WINDOW_ID = "lbh.tree";
    static constexpr const wchar_t* WINDOW_DISPLAY_NAME = L"树形图";

    TreeWindow();
    virtual ~TreeWindow() override;

    virtual void OnRender() override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;

    virtual void Serialize(json& o) override;
    virtual void Deserialize(json& o) override;

    void RenderItem(AViewObject* o);
};

}

#endif