#ifndef __VIEWMGR__
#define __VIEWMGR__

#include "define.h"

#include "list.h"
#include "gltools.h"

class ViewManager final : public Object {
private:
    AppFrame* frame;

    List<GLRect> rects;
    GLRect curRect;

public:
    static ViewManager* GetLocalInst();

    ViewManager(AppFrame* frame);
    ~ViewManager();
    void Reset();
    void SetView(GLRect rect);
    void PushView(GLRect rect);
    void PopView();
    GLRect GetRect();
    float GetWidth();
    float GetHeight();
    float GetAspect();
    Vector2 GetSize();
    GLRect CalcChildRect(GLRect ratio);
    void PushChildView(GLRect rect);
    void SetChildScissor(GLRect ratio);

    void EnableScissor();
    void DisableScissor();
    void SetScissor(GLRect rect);
    void ResetScissor();
};

#endif