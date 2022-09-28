#ifndef __EDITOR_WINDOWS_TREEWINDOWOBJECT__
#define __EDITOR_WINDOWS_TREEWINDOWOBJECT__

#include <define.h>

#include <util/List.h>
#include <editor/object/WindowObject.h>

namespace simple3deditor {

class TreeWindowObject final : public AWindowObject {
private:
    List<AViewObject*> objectList;
    int depth;

    Menu* basicMenu;

    AViewObject* dragObject = NULL;
    AViewObject* selObject = NULL;

    void AddObject(AViewObject* o);

protected:
    void UpdateCursor(Point2 pos);
    void UpdateWindowSize(Vector2 size);

public:
    OBJECT_INFO_DEF();

    TreeWindowObject();
    virtual ~TreeWindowObject() override;

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnRender() override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(Vector2 size) override;
    virtual void OnMouseMove2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnLeftUp2D(Point2 pos) override;
    virtual void OnRightDown2D(Point2 pos) override;
    virtual void OnRightUp2D(Point2 pos) override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;

    void RenderItem(AViewObject* o);
};

}

#endif