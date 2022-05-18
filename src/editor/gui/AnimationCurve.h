#ifndef __EDITOR_GUI_ANIMATIONCURVE__
#define __EDITOR_GUI_ANIMATIONCURVE__

#include <define.h>

#include <functional>

#include <util/List.h>
#include <util/math3d/LinearAlgebra.h>
#include <editor/gui/UIManager.h>

namespace simple3deditor {

using AnimationFunction = std::function<float(Vector2, Vector2, float)>;

class AnimationCurve : public IButton {
private:
    enum SelectTarget {
        NONE,
        POINT,
        SEGMENT
    };

    static constexpr float BOUND_TOP = 0.8f;
    static constexpr float BOUND_BOTTOM = -0.8f;
    static constexpr float ERROR_NUM = 10.0f;
    static constexpr float DEPTH = -1.0f;

    float startFrame;
    float endFrame;

    Vector2 cursorPos;
    
    List<AnimationFunction> functions;
    List<Vector2> points;

    SelectTarget selTarget = NONE;
    size_t selIndex;
    Vector2 initialPos;

    Menu* basicMenu;

    float ratio = 1.0f;

public:
    AnimationCurve(float startFrame, float endFrame);
    ~AnimationCurve();

    virtual bool Trigger(Vector2 pos) override;
    virtual void Hover(Vector2 pos) override;
    virtual void Click(Vector2 pos) override;
    virtual void Drag(Vector2 dir) override;
    virtual void OnWheel(int delta) override;
    virtual void Render() override;

    size_t GetSegment(float pos);
    float GetValue(float pos);
    void OnChangeRange(float start, float end);
    void SetFunc(size_t seg, AnimationFunction func);
    void AddPoint(Vector2 point);
    void AddPoint(size_t index, Vector2 point);
    void RemovePoint(size_t index);
    void FlushRange();
};

}

#endif