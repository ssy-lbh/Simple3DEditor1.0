#ifndef __EDITOR_OBJECT_WINDOWOBJECT__
#define __EDITOR_OBJECT_WINDOWOBJECT__

#include <define.h>

#include <editor/object/GUIObject.h>

namespace simple3deditor {

class AWindowObject : public AGUIObject {
protected:
    Vector2 cursorCoord;
    Vector2 cursorPos = Vector2::zero;
    Vector2 cliSize;
    Vector2 cliInvSize;
    float aspect;

    AWindowObject(const wchar_t* name);

    void UpdateCursor(Point2 pos);
    void UpdateWindowSize(Vector2 size);

public:
    OBJECT_INFO_DEF();

    AWindowObject();
    virtual ~AWindowObject() override;

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnRender() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(Vector2 size) override;
    virtual void OnMouseMove2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnLeftUp2D(Point2 pos) override;
    virtual void OnRightDown2D(Point2 pos) override;
    virtual void OnRightUp2D(Point2 pos) override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path, uint len) override;
    virtual void OnDropFileW(const wchar_t* path, uint len) override;
};

}

#endif