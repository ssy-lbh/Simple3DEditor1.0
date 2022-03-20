#ifndef __EDITOR_MAIN_VIEWMANAGER__
#define __EDITOR_MAIN_VIEWMANAGER__

#include <define.h>

#include <utils/List.h>
#include <utils/math3d/LinearAlgebra.h>

class ViewManager final : public Object {
private:
    AppFrame* frame;

    List<Rect> rects;
    Rect curRect;

public:
    static ViewManager* GetLocalInst();

    ViewManager(AppFrame* frame);
    ~ViewManager();
    void Reset();
    void SetView(Rect rect);
    void PushView(Rect rect);
    void PopView();
    Rect GetRect();
    float GetWidth();
    float GetHeight();
    float GetAspect();
    Vector2 GetSize();
    Rect CalcChildRect(Rect ratio);
    void PushChildView(Rect rect);
    void SetChildScissor(Rect ratio);

    void EnableScissor();
    void DisableScissor();
    void SetScissor(Rect rect);
    void ResetScissor();
};

#endif