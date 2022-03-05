#ifndef __EDITOR_ANIMATIONWINDOW__
#define __EDITOR_ANIMATIONWINDOW__

#include <define.h>

#include <editor/gui/UIManager.h>
#include <editor/gui/Menu.h>
#include <utils/List.h>

class AnimationWindow : public IWindow {
private:
    static const float DEFAULT_START_FRAME;
    static const float DEFAULT_END_FRAME;

    bool focus;

    Vector2 cliSize, cliInvSize;
    float aspect;
    Vector2 cursorPos;

    UIManager* uiMgr;

    Menu* basicMenu = NULL;

    float startFrame = DEFAULT_START_FRAME;
    float endFrame = DEFAULT_END_FRAME;
    float fps = 25.0f;
    float frame = 0.0f;
    bool play = false;

    float time;

    AnimationCurve* curve = NULL;

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
    private:
        static const Vector3 COLOR;
        static const float DEPTH;
        static const float BOUND_TOP;
        static const float BOUND_BOTTOM;

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

    class RotationMenu : public IMenuItem {
    private:
        AnimationWindow* window;
        Menu* xyzMenu;
        Menu* quatMenu;

    public:
        RotationMenu(AnimationWindow* window);
        virtual ~RotationMenu() override;

        virtual IMenuItem::ItemType GetType() override;
        virtual const wchar_t* GetName() override;
        virtual Menu* GetMenu() override;
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
    void SetCurve(AnimationCurve* curve);
    void SetProperty(Property* prop);
    void SetFrame(float frame);

    void Play();
    void Stop();
    bool IsPlaying();
};

#endif