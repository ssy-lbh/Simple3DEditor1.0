#ifndef __EDITOR_AUDIOCAPTUREWINDOW__
#define __EDITOR_AUDIOCAPTUREWINDOW__

#include <define.h>

#include <editor/gui/UIManager.h>
#include <editor/gui/Menu.h>
#include <utils/String.h>
#include <utils/math3d/LinearAlgebra.h>

typedef struct ALCdevice_struct ALCdevice;

class AudioCaptureWindow final : public AWindow {
private:
    static const int SAMPLE_SIZE_BIT = 14;
    static const int SAMPLE_SIZE = (1 << SAMPLE_SIZE_BIT);
    static const int SAMPLE_MASK = (SAMPLE_SIZE - 1);
    static const int QUEUE_SIZE_BIT = 2;
    static const int QUEUE_SIZE = (1 << QUEUE_SIZE_BIT);
    static const int QUEUE_MASK = (QUEUE_SIZE - 1);
    static const int FREQUENCY = 44100;

    UIManager* uiMgr;

    Menu* basicMenu;

    ALCdevice* capDev = NULL;
    void* capBuf = NULL;
    int capOffset = 0;
    void* recBuf = NULL;
    int recOffset = 0;
    Complex* freqBuf = NULL;

    bool capture = false;

    uint alBuf[QUEUE_SIZE];
    uint alSrc;
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
        virtual void Hover(Vector2 pos) override;
        virtual void Leave(Vector2 pos) override;
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

protected:
    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    AudioCaptureWindow();
    virtual ~AudioCaptureWindow() override;

    void ProcessInput();
    void ProcessOutput();

    virtual void OnRender() override;
    virtual void OnCreate() override;
    virtual void OnClose() override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path) override;
    virtual void OnDropFileW(const wchar_t* path) override;

    void Launch();
    void Stop();
    void UpdateBuffer(void* buf, int size);
    bool SetCaptureDevice(const char* name);
    bool CloseCaptureDevice();
};

#endif