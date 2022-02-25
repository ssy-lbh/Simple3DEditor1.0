#ifndef __ANIM__
#define __ANIM__

#include "define.h"
#include "list.h"
#include "uimgr.h"
#include "menu.h"

interface IAnimationFunction {
public:
    IAnimationFunction();
    virtual ~IAnimationFunction();

    virtual float GetValue(Vector2 p1, Vector2 p2, float val);
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

class AnimationWindow : public IWindow {
private:
    bool focus;

    Vector2 cliSize, cliInvSize;
    float aspect;
    Vector2 cursorPos;

    UIManager* uiMgr;

    Menu* basicMenu = NULL;

    float startFrame = 0.0f;
    float endFrame = 250.0f;
    float fps = 25.0f;
    float frame = 0.0f;
    bool play = false;

    class FrameIndicator : public IButton {
    private:
        AnimationWindow* window;
        float pos = 0.0f;
        float origin;
    
    public:
        FrameIndicator(AnimationWindow* window);
        virtual ~FrameIndicator() override;

        virtual bool Trigger(Vector2 pos) override;
        virtual void Click(Vector2 pos) override;
        virtual void Drag(Vector2 dir) override;
        virtual void Render() override;
    };

    class Bottom : public IButton {
    public:
        Bottom();
        virtual ~Bottom() override;

        virtual void Render() override;
    };

    class PlayButton : public IButton {
    private:
        AnimationWindow* window;
    
    public:
        PlayButton(AnimationWindow* window);
        virtual ~PlayButton() override;

        virtual bool Trigger(Vector2 pos) override;
        virtual void Click(Vector2 pos) override;
        virtual void Render() override;
    };

    class AnimationCurve : public IButton {
    private:
        enum SelectTarget {
            NONE,
            POINT,
            SEGMENT
        };

        AnimationWindow* window;
        
        List<IAnimationFunction*> functions;
        List<Vector2> points;

        SelectTarget selTarget = NONE;
        size_t selIndex;
        Vector2 initialPos;

        Menu* funcMenu;

    public:
        AnimationCurve(AnimationWindow* window);
        ~AnimationCurve();

        virtual bool Trigger(Vector2 pos) override;
        virtual void Click(Vector2 pos) override;
        virtual void Drag(Vector2 dir) override;
        virtual void Render() override;

        size_t GetSegment(float pos);
        float GetValue(float pos);
        void OnChangeRange(float start, float end);
        void SetFunc(size_t seg, IAnimationFunction* func);
    };

    AnimationCurve* curve;

public:
    AnimationWindow();
    virtual ~AnimationWindow() override;

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnTimer(int id) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;

    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);
};

#endif