#ifndef __EDITOR_TREEWINDOW__
#define __EDITOR_TREEWINDOW__

#include <define.h>

#include <editor/gui/UIManager.h>

class TreeWindow final : public IWindow {
private:
    bool focus = false;

    Vector2 cliSize, cliInvSize;
    Vector2 cursorPos;

    List<AViewObject*> objectList;
    int depth;

    Menu* basicMenu;

    AViewObject* dragObject = NULL;
    AViewObject* selObject = NULL;

    void AddObject(AViewObject* o);

public:
    TreeWindow();
    virtual ~TreeWindow() override;

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnClose() override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMouseHover(int key, int x, int y) override;
    virtual void OnMouseLeave() override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;

    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);
    void RenderItem(AViewObject* o);
};

#endif