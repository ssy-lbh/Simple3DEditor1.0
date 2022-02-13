#ifndef __ANIM__
#define __ANIM__

#include "list.h"
#include "uimgr.h"

class AnimationCurve;
class IAnimationInterp;
class IAnimationModifier;
class AnimationWindow;

class AnimationCurve {
private:
    List<IAnimationInterp*> interp;
    List<int> nodes;

public:
    AnimationCurve();
    ~AnimationCurve();

    float GetValue(int frame);
    void Reset(float value, int frameLen);
    void SetLength(int frameLen);
    int InsertNode(int frame);
    void OnRender();
};

class IAnimationInterp {
public:
    IAnimationInterp();
    IAnimationInterp(int start, int end);
    virtual ~IAnimationInterp();

    void SetStartFrame(int frame);
    void SetEndFrame(int frame);
    float GetValue(int frame);
    void OnRender();
};

class IAnimationModifier {
public:
    IAnimationModifier(int start, int end);
    virtual ~IAnimationModifier();

    float GetValue(int frame);
};

class AnimationWindow : public IWindow {
private:
    bool focus;

    Vector2 cliSize, cliInvSize;
    float aspect;
    Vector2 cursorPos;

    UIManager* uiMgr;

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
        virtual void Click() override;
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
        virtual void Click() override;
        virtual void Render() override;
    };

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