#ifndef __EDITOR_AUDIOCAPTUREWINDOW__
#define __EDITOR_AUDIOCAPTUREWINDOW__

#include <define.h>

#include <util/String.h>
#include <util/math3d/LinearAlgebra.h>
#include <editor/main/Window.h>

typedef struct ALCdevice_struct ALCdevice;

namespace simple3deditor {

class AudioCaptureWindow final : public AWindow {
private:
    static constexpr uint SAMPLE_SIZE_BIT = 14;
    static constexpr uint SAMPLE_SIZE = (1 << SAMPLE_SIZE_BIT);
    static constexpr uint SAMPLE_MASK = (SAMPLE_SIZE - 1);
    static constexpr uint QUEUE_SIZE_BIT = 2;
    static constexpr uint QUEUE_SIZE = (1 << QUEUE_SIZE_BIT);
    static constexpr uint QUEUE_MASK = (QUEUE_SIZE - 1);
    static constexpr uint FREQUENCY = 44100;

    GUIManagerObject* guiMgr;

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

protected:
    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    static constexpr const char* WINDOW_ID = "lbh.audiocap";
    static constexpr const wchar_t* WINDOW_DISPLAY_NAME = L"变声器";

    AudioCaptureWindow();
    virtual ~AudioCaptureWindow() override;

    void ProcessInput();
    void ProcessOutput();

    virtual void OnRender() override;
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

    virtual void Serialize(IOutputStream& os) override;
    virtual void Deserialize(IInputStream& os) override;

    void Launch();
    void Stop();
    void UpdateBuffer(void* buf, int size);
    bool SetCaptureDevice(const char* name);
    bool CloseCaptureDevice();
};

}

#endif