#ifndef __EDITOR_AUDIOPLAYERWINDOW__
#define __EDITOR_AUDIOPLAYERWINDOW__

#include <define.h>

#include <editor/gui/UIManager.h>
#include <editor/gui/Menu.h>

struct AudioWaveFormat {
    short wFormatTag;
    short nChannels;
    int nSamplesPerSec;
    int nAvgBytesPerSec;
    short nBlockAlign;
    short wBitsPerSample;
};

class AudioPlayerWindow final : public IWindow {
private:
    const int bit = 12;
    
    bool focus = false;
    Vector2 size = Vector2::one;
    Vector2 cursorPos;

    UIManager* uiMgr;

    Menu* basicMenu;

    AudioSourceObject* source = NULL;

    bool displayWave = false;

    class PlayButton final : public IButton {
    private:
        static const float BOUND_LEFT;
        static const float BOUND_RIGHT;
        static const float BOUND_TOP;
        static const float BOUND_BOTTOM;
        static const float RADIUS;
        static const Vector3 HOVER_COLOR;
        static const Vector3 LEAVE_COLOR;

        AudioPlayerWindow* window;
        bool hover = false;

    public:
        PlayButton(AudioPlayerWindow* window);
        virtual ~PlayButton() override;

        virtual bool Trigger(Vector2 pos) override;
        virtual void Hover(Vector2 pos) override;
        virtual void Click(Vector2 pos) override;
        virtual void Leave(Vector2 pos) override;
        virtual void Render() override;
    };

    class ProgressBar final : public IButton {
    private:
        static const float LOW_BOUND;
        static const float HIGH_BOUND;
        static const float POSITION_Y;
        static const float BUTTON_WIDTH_X;
        static const float BUTTON_WIDTH_Y;

        AudioPlayerWindow* window;
        bool hover = false;
        float pos = LOW_BOUND;
        float origin;
    
    public:
        ProgressBar(AudioPlayerWindow* window);
        virtual ~ProgressBar() override;

        virtual bool Trigger(Vector2 pos) override;
        virtual void Click(Vector2 pos) override;
        virtual void Drag(Vector2 dir) override;
        virtual void Hover(Vector2 pos) override;
        virtual void Leave(Vector2 pos) override;
        virtual void Render() override;
    };

    class GainBar final : public IButton {
    private:
        static const float LOW_BOUND;
        static const float HIGH_BOUND;
        static const float POSITION_X;
        static const float BUTTON_WIDTH_X;
        static const float BUTTON_WIDTH_Y;

        AudioPlayerWindow* window;
        bool hover = false;
        float pos = HIGH_BOUND;
        float origin;
    
    public:
        GainBar(AudioPlayerWindow* window);
        virtual ~GainBar() override;

        virtual bool Trigger(Vector2 pos) override;
        virtual void Click(Vector2 pos) override;
        virtual void Drag(Vector2 dir) override;
        virtual void Hover(Vector2 pos) override;
        virtual void Leave(Vector2 pos) override;
        virtual void Render() override;
    };

    class LoopOption final : public IButton {
    private:
        AudioPlayerWindow* window;
        bool loop;
    
    public:
        LoopOption(AudioPlayerWindow* window);
        virtual ~LoopOption() override;

        virtual bool Trigger(Vector2 pos) override;
        virtual void Click(Vector2 pos) override;
        virtual void Render() override;
    };

    class LoopItem final : public IMenuItem {
    private:
        AudioPlayerWindow* window;

    public:
        LoopItem(AudioPlayerWindow* window);
        virtual ~LoopItem() override;

        virtual const wchar_t* GetName() override;

        virtual void OnClick() override;
    };

    class DopplerEffectItem final : public IMenuItem {
    private:
        AudioPlayerWindow* window;

    public:
        DopplerEffectItem(AudioPlayerWindow* window);
        virtual ~DopplerEffectItem() override;

        virtual const wchar_t* GetName() override;

        virtual void OnClick() override;
    };

public:
    AudioPlayerWindow();
    virtual ~AudioPlayerWindow() override;

    void DrawLineGraph(float* height, size_t size);
    void DrawAmplitudeGraph(float* height, size_t size);
    void DrawTime();
    void RenderGraph();

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnClose() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMouseHover(int key, int x, int y) override;
    virtual void OnMouseLeave() override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path) override;
    virtual void OnDropFileW(const wchar_t* path) override;

    void OnInsLoad();

    void PreloadFile(WString file);
    void LoadFile(WString file);
    void LoadObject(AudioSourceObject* o);
    bool IsLoaded();
    void Launch();
    void Stop();
    bool IsLaunched();
    bool IsLoop();
    void SetLoop(bool loop);
    int GetOffset();
    bool HasDopplerEffect();
    void SetDopplerEffect(bool on);

    int GetWaveFormat(AudioWaveFormat* wav);
};

#endif