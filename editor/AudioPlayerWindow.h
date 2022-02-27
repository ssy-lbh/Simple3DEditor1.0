#ifndef __EDITOR_AUDIOPLAYERWINDOW__
#define __EDITOR_AUDIOPLAYERWINDOW__

#include <define.h>

#include <lib/openal/al.h>
#include <lib/openal/alc.h>

#include <utils/AudioUtils.h>
#include <utils/String.h>
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
    bool focus = false;

    Vector2 size = Vector2::one;
    Vector2 cursorPos;

    UIManager* uiMgr;

    Menu* basicMenu;

    WString path;

    bool loaded = false;
    bool launched = false;

    ALuint alSrc;
    ALuint alBuf;

    /* size in samples */
    int alAudioSize;
    /* length in seconds */
    int alAudioLen;
    /* size in bytes */
    int alSampleSize;
    /* number of channels */
    int alChannels;
    /* audio data */
    void* alAudioData;
    ALint alAudioOffset;

    bool displayWave = false;

    class PlayButton final : public IButton {
    private:
        AudioPlayerWindow* window;
        bool hover = false;

    public:
        PlayButton(AudioPlayerWindow* window);
        virtual ~PlayButton() override;

        virtual bool Trigger(Vector2 pos) override;
        virtual void Hover() override;
        virtual void Click(Vector2 pos) override;
        virtual void Leave() override;
        virtual void Render() override;
    };

    class ProgressBar final : public IButton {
    private:
        AudioPlayerWindow* window;
        bool hover = false;
        float pos = -0.6f;
        float origin;
    
    public:
        ProgressBar(AudioPlayerWindow* window);
        virtual ~ProgressBar() override;

        virtual bool Trigger(Vector2 pos) override;
        virtual void Click(Vector2 pos) override;
        virtual void Drag(Vector2 dir) override;
        virtual void Hover() override;
        virtual void Leave() override;
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

    class DisplayModeItem final : public IMenuItem {
    private:
        AudioPlayerWindow* window;

    public:
        DisplayModeItem(AudioPlayerWindow* window);
        virtual ~DisplayModeItem() override;

        virtual const wchar_t* GetName() override;

        virtual void OnClick() override;
    };

public:
    AudioPlayerWindow();
    virtual ~AudioPlayerWindow() override;

    void DrawLineGraph(float* height, size_t size);
    void DrawAmplitudeGraph(float* height, size_t size);
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
    bool IsLoaded();
    void Launch();
    void Stop();
    bool IsLaunched();
    bool IsLoop();
    void SetLoop(bool loop);
    ALint GetOffset();

    ALint GetWaveFormat(AudioWaveFormat* wav);
};

#endif