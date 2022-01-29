#ifndef __AUDIO__
#define __AUDIO__

#include <windef.h>
#include <mmsystem.h>

#include "openal/al.h"
#include "openal/alc.h"

#include "uimgr.h"

#include "soundtouch/SoundTouch.h"

class AudioUtils;
class AudioPlayerWindow;
class AudioCaptureWindow;

class AudioUtils {
public:
    static ALCdevice* alDev;
    static ALCcontext* alCtx;

    union Complex {
        _Complex float comp;
        float real, imag;

        Complex(_Complex float comp);
        ~Complex();

        float MagnitudeSqr();
        float Magnitude();
    };

    static bool CheckALError(const char* tag, const char* file, int line);
    static void FFT(_Complex float* input, int sizebit, bool inv);
    static void InitOpenAL();
    static void UninitOpenAL();
};

class AudioPlayerWindow : public IWindow {
private:
    bool focus = false;

    Vector2 size = Vector2::one;
    Vector2 cursorPos;

    UIManager* uiMgr;

    wchar_t path[MAX_PATH];

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

    class PlayButton : public IButton {
    private:
        AudioPlayerWindow* window;
        bool hover = false;

    public:
        PlayButton(AudioPlayerWindow* window);
        virtual ~PlayButton() override;

        virtual bool Trigger(Vector2 pos) override;
        virtual void Hover() override;
        virtual void Click() override;
        virtual void Leave() override;
        virtual void Render() override;
    };

    class ProgressBar : public IButton {
    private:
        AudioPlayerWindow* window;
        bool hover = false;
        float pos = -0.6f;
        float origin;
    
    public:
        ProgressBar(AudioPlayerWindow* window);
        virtual ~ProgressBar() override;

        virtual bool Trigger(Vector2 pos) override;
        virtual void Click() override;
        virtual void Drag(Vector2 dir) override;
        virtual void Hover() override;
        virtual void Leave() override;
        virtual void Render() override;
    };

    class LoopOption : public IButton {
        private:
        AudioPlayerWindow* window;
        bool loop = false;
    
    public:
        LoopOption(AudioPlayerWindow* window);
        virtual ~LoopOption() override;

        virtual bool Trigger(Vector2 pos) override;
        virtual void Click() override;
        virtual void Render() override;
    };

public:
    AudioPlayerWindow();
    virtual ~AudioPlayerWindow() override;

    void RenderFreqGraph();

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

    void PreloadFileW(const wchar_t* file);
    void LoadFileW(const wchar_t* file);
    bool IsLoaded();
    void Launch();
    void Stop();
    bool IsLaunched();

    ALint GetWaveFormat(PWAVEFORMATEX lpwav);
};

class AudioCaptureWindow : public IWindow {
private:
    static const int bit = 14;
    static const int queueBit = 2;
    static const int queueMask = ((1 << queueBit) - 1);

    static const int freq = 44100;

    bool focus = false;

    Vector2 size;
    Vector2 cursorPos;

    ALCdevice* capDev = NULL;
    ALvoid* capBuf = NULL;
    _Complex float* freqBuf = NULL;
    soundtouch::SAMPLETYPE* playBuf = NULL;

    bool capture = false;

    ALuint alBuf[1 << queueBit];
    ALuint alSrc;
    int head = 0, tail = 0;

    soundtouch::SoundTouch* soundTouch = NULL;

public:
    AudioCaptureWindow();
    virtual ~AudioCaptureWindow() override;

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