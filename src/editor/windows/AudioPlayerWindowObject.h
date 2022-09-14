#ifndef __EDITOR_WINDOWS_AUDIOPLAYERWINDOWOBJECT__
#define __EDITOR_WINDOWS_AUDIOPLAYERWINDOWOBJECT__

#include <define.h>

#include <editor/object/WindowObject.h>
#include <editor/object/AudioSourceObject.h>
#include <editor/AudioPlayerWindow.h>

namespace simple3deditor {

class AudioPlayerWindowObject final : public AWindowObject, public CAudioSourceLoader {
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
    OBJECT_INFO_DEF();

    AudioPlayerWindowObject();
    virtual ~AudioPlayerWindowObject() override;

    void DrawLineGraph(float* height, size_t size);
    void DrawAmplitudeGraph(float* height, size_t size);
    void DrawTime();
    void RenderGraph();

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnRender() override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(Vector2 size) override;
    virtual void OnMouseMove2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnLeftUp2D(Point2 pos) override;
    virtual void OnRightDown2D(Point2 pos) override;
    virtual void OnRightUp2D(Point2 pos) override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path, uint len) override;
    virtual void OnDropFileW(const wchar_t* path, uint len) override;

    void OnInsLoad();

    void PreloadFile(WString file);
    void LoadFile(WString file);
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