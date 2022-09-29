#ifndef __EDITOR_ANIMATIONWINDOW__
#define __EDITOR_ANIMATIONWINDOW__

#include <define.h>

#include <editor/gui/UIManager.h>
#include <editor/gui/Menu.h>
#include <util/List.h>

namespace simple3deditor {

class AnimationWindow : public AWindow {
private:
    static constexpr float DEFAULT_START_FRAME = 0.0f;
    static constexpr float DEFAULT_END_FRAME = 250.0f;

    UIManager* uiMgr;

    Menu* basicMenu = NULL;

    float startFrame = DEFAULT_START_FRAME;
    float endFrame = DEFAULT_END_FRAME;
    float fps = 25.0f;
    float frame = 0.0f;
    bool play = false;

    AnimationCurve* curve = NULL;

    class FrameIndicator;
    class Bottom;
    class PlayButton;

    friend class FrameIndicator;
    friend class Bottom;
    friend class PlayButton;

protected:
    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    WINDOW_INFO_DEF();

    AnimationWindow();
    virtual ~AnimationWindow() override;

    virtual void OnRender() override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;

    virtual void Serialize(nlohmann::json& o) override;
    virtual void Deserialize(nlohmann::json& o) override;
    
    void SetCurve(AnimationCurve* curve);
    void SetProperty(Property* prop);
    void SetFrame(float frame);

    void Play();
    void Stop();
    bool IsPlaying();
};

}

#endif