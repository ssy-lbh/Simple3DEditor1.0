#ifndef __EDITOR_OBJECT_GUIMANAGEROBJECT__
#define __EDITOR_OBJECT_GUIMANAGEROBJECT__

#include <define.h>

#include <editor/main/ViewObject.h>
#include <editor/object/GUIObject.h>

namespace simple3deditor {

// 3D GUI管理器已经基本可用
class GUIManagerObject : public AGUIObject {
protected:
    AGUIObject* focus = NULL;
    AGUIObject* cur = NULL;
    Point2 startPos;
    Point2 cursorPos;
    bool leftDown = false;
    bool rightDown = false;

    GUIManagerObject(const wchar_t* name, ViewObjectType type);

    virtual bool UpdateCursor3D(Point3 ori, Vector3 dir);
    void UpdateCursor(Vector2 pos);

public:
    GUIManagerObject();
    virtual ~GUIManagerObject() override;

    virtual void AddChild(AViewObject* o) override;
    virtual bool DeleteChild(AViewObject* o) override;

    virtual void OnRender() override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnMouseMove(Point3 ori, Vector3 dir) override;
    virtual void OnLeftDown(Point3 ori, Vector3 dir) override;
    virtual void OnLeftUp(Point3 ori, Vector3 dir) override;
    virtual void OnRightDown(Point3 ori, Vector3 dir) override;
    virtual void OnRightUp(Point3 ori, Vector3 dir) override;
    virtual void OnMouseMove2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnLeftUp2D(Point2 pos) override;
    virtual void OnRightDown2D(Point2 pos) override;
    virtual void OnRightUp2D(Point2 pos) override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path, uint len) override;
    virtual void OnDropFileW(const wchar_t* path, uint len) override;

    AGUIObject* GetCurrent();
    AGUIObject* FindCurrent();
};

}

#endif