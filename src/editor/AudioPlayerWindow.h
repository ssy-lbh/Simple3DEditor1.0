#ifndef __EDITOR_AUDIOPLAYERWINDOW__
#define __EDITOR_AUDIOPLAYERWINDOW__

#include <define.h>

#include <editor/main/Window.h>
#include <editor/object/AudioSourceObject.h>

namespace simple3deditor {

struct AudioWaveFormat {
    short wFormatTag;
    short nChannels;
    int nSamplesPerSec;
    int nAvgBytesPerSec;
    short nBlockAlign;
    short wBitsPerSample;
};

class AudioPlayerWindow final : public AWindow, public CAudioSourceLoader {
private:
    static constexpr uint SAMPLE_SIZE_BIT = 12;
    static constexpr uint SAMPLE_SIZE = (1 << SAMPLE_SIZE_BIT);
    static constexpr uint SAMPLE_MASK = (SAMPLE_SIZE - 1);
    static constexpr uint GRAPH_SIZE_BIT = 10;
    static constexpr uint GRAPH_SIZE = (1 << GRAPH_SIZE_BIT);

    GUIManagerObject* guiMgr;

    Menu* basicMenu;

    AudioSourceObject* source = NULL;

    bool displayWave = false;

public:
    WINDOW_INFO_DEF();

    AudioPlayerWindow();
    virtual ~AudioPlayerWindow() override;

    void DrawLineGraph(float* height, size_t size);
    void DrawAmplitudeGraph(float* height, size_t size);
    void DrawTime();
    void RenderGraph();

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
    virtual void OnDropFileA(const char* path, uint len) override;
    virtual void OnDropFileW(const wchar_t* path, uint len) override;

    virtual void Serialize(nlohmann::json& o) override;
    virtual void Deserialize(nlohmann::json& o) override;

    void OnInsLoad();

    void PreloadFile(String& file);
    void LoadFile(WString file);
    void LoadObject(AudioSourceObject* o);
    bool IsLoaded();
    void Launch();
    void Stop();
    bool IsLaunched();
    bool IsLoop();
    void SetLoop(bool loop);
    int GetOffset();
    float GetOffsetRate();
    void SetOffsetRate(float rate);
    void SetGain(float gain);
    bool HasDopplerEffect();
    void SetDopplerEffect(bool on);

    int GetWaveFormat(AudioWaveFormat* wav);
};

}

#endif