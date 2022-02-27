#ifndef __EDITOR_AUDIOCAPTUREWINDOW__
#define __EDITOR_AUDIOCAPTUREWINDOW__

#include <define.h>

#include <lib/openal/al.h>
#include <lib/openal/alc.h>

#include <utils/AudioUtils.h>
#include <utils/String.h>
#include <editor/gui/UIManager.h>
#include <editor/gui/Menu.h>

#include <lib/soundtouch/SoundTouch.h>

class AudioCaptureWindow final : public IWindow {
private:
    static const int bit = 14;
    static const int queueBit = 2;
    static const int queueMask = ((1 << queueBit) - 1);

    static const int freq = 44100;

    bool focus = false;

    Vector2 size;
    Vector2 cursorPos;

    UIManager* uiMgr;

    Menu* basicMenu;

    ALCdevice* capDev = NULL;
    ALvoid* capBuf = NULL;
    ALint capOffset = 0;
    ALvoid* recBuf = NULL;
    ALint recOffset = 0;
    _Complex float* freqBuf = NULL;

    bool capture = false;

    ALuint alBuf[1 << queueBit];
    ALuint alSrc;
    int head = 0, tail = 0;

    soundtouch::SoundTouch* soundTouch = NULL;

    bool displayWave = false;
    bool adjustWave = false;
    int ratio = 1;

    class ProgressBar final : public IButton {
    private:
        AudioCaptureWindow* window;
        bool hover = false;
        float pos = 0.0f;
        float origin;
    
    public:
        ProgressBar(AudioCaptureWindow* window);
        virtual ~ProgressBar() override;

        virtual bool Trigger(Vector2 pos) override;
        virtual void Click(Vector2 pos) override;
        virtual void Drag(Vector2 dir) override;
        virtual void Hover() override;
        virtual void Leave() override;
        virtual void Render() override;
    };

    class CaptureItem final : public IMenuItem {
    private:
        AudioCaptureWindow* window;

    public:
        CaptureItem(AudioCaptureWindow* window);
        virtual ~CaptureItem() override;

        virtual const wchar_t* GetName() override;

        virtual void OnClick() override;
    };

    class DisplayModeItem final : public IMenuItem {
    private:
        AudioCaptureWindow* window;

    public:
        DisplayModeItem(AudioCaptureWindow* window);
        virtual ~DisplayModeItem() override;

        virtual const wchar_t* GetName() override;

        virtual void OnClick() override;
    };

    class AdjushWaveItem final : public IMenuItem {
    private:
        AudioCaptureWindow* window;

    public:
        AdjushWaveItem(AudioCaptureWindow* window);
        virtual ~AdjushWaveItem() override;

        virtual const wchar_t* GetName() override;

        virtual void OnClick() override;
    };

public:
    AudioCaptureWindow();
    virtual ~AudioCaptureWindow() override;

    void ProcessInput();
    void ProcessOutput();

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnCreate() override;
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

    void Launch();
    void Stop();
    void UpdateBuffer(ALvoid* buf, ALsizei size);
};

#endif