#ifndef __EDITOR_NODEMAPWINDOW__
#define __EDITOR_NODEMAPWINDOW__

#include <define.h>

#include <util/math3d/LinearAlgebra.h>
#include <editor/main/Window.h>
#include <editor/object/GUIObject.h>

namespace simple3deditor {

class Node;

class NodeMapWindow final : public AWindow {
private:
    Vector2 viewPos;

    GUIManagerObject* guiMgr;
    GUIManagerObject* nodeMgr;

    Menu* basicMenu;

    GLTexture2D* bktex = NULL;

    class MoveButton;
    class Node;

    friend class MoveButton;
    friend class Node;

    List<Node*> selectedNodes;

protected:
    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    WINDOW_INFO_DEF();

    NodeMapWindow();
    virtual ~NodeMapWindow() override;

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

    virtual void Serialize(nlohmann::json& o) override;
    virtual void Deserialize(nlohmann::json& o) override;

    void AddNode();
};

}

#endif