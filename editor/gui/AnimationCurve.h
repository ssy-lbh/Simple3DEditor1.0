#ifndef __EDITOR_GUI_ANIMATIONCURVE__
#define __EDITOR_GUI_ANIMATIONCURVE__

#include <define.h>

#include <editor/gui/UIManager.h>
#include <utils/List.h>

interface IAnimationFunction {
public:
    IAnimationFunction();
    virtual ~IAnimationFunction();

    virtual float GetValue(Vector2 p1, Vector2 p2, float val);
};

class RightValueFunc final : public IAnimationFunction {
public:
    RightValueFunc();
    virtual ~RightValueFunc() override;

    virtual float GetValue(Vector2 p1, Vector2 p2, float val) override;
};

class LinearFunc final : public IAnimationFunction {
public:
    LinearFunc();
    virtual ~LinearFunc() override;

    virtual float GetValue(Vector2 p1, Vector2 p2, float val) override;
};

class SquareFunc final : public IAnimationFunction {
public:
    SquareFunc();
    virtual ~SquareFunc() override;

    virtual float GetValue(Vector2 p1, Vector2 p2, float val) override;
};

class AnimationCurve : public IButton {
private:
    enum SelectTarget {
        NONE,
        POINT,
        SEGMENT
    };

    static const float BOUND_TOP;
    static const float BOUND_BOTTOM;
    // size in pixels
    static const float ERROR_NUM;
    static const float DEPTH;

    float startFrame;
    float endFrame;

    Vector2 cursorPos;
    
    List<IAnimationFunction*> functions;
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
    void SetFunc(size_t seg, IAnimationFunction* func);
    void AddPoint(Vector2 point);
    void AddPoint(size_t index, Vector2 point);
    void RemovePoint(size_t index);
    void FlushRange();
};

#endif