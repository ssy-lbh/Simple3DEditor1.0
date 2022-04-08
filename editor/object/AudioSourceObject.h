#ifndef __EDITOR_OBJECT_AUDIOSOURCEOBJECT__
#define __EDITOR_OBJECT_AUDIOSOURCEOBJECT__

#include <define.h>

#include <utils/math3d/LinearAlgebra.h>
#include <editor/main/ViewObject.h>

class IAudioSourceLoader {
public:
    virtual void AudioSourceLoad(AudioSourceObject* o) = 0;
};

class CAudioSourceLoader : public IAudioSourceLoader {
private:
    AudioSourceObject*& ref;

public:
    CAudioSourceLoader(AudioSourceObject*& ref);
    ~CAudioSourceLoader();

    virtual void AudioSourceLoad(AudioSourceObject* o) override;
};

class AudioSourceObject final : public AViewObject {
private:
    uint alSrc;
    uint alBuf;

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
    int alAudioFreq;
    int alAudioOffset;

    Point3 recPos;
    bool dopplerEffect = true;

    IAudioSourceLoader* loader = NULL;

public:
    // 输入的数据内部引用，自动回收，不能调用后释放data所在内存
    AudioSourceObject(uenum format, char* data, int size, int freq);
    virtual ~AudioSourceObject() override;

    // 使用渲染回调获取最新位置信息，并更新AudioListener
    virtual void OnRender() override;

    char* GetData();
    int GetSize();
    int GetLength();
    int GetSampleSize();
    int GetChannelCount();
    int GetFrequency();
    int GetOffset();
    bool IsLoop();
    float GetGain();
    IAudioSourceLoader* GetLoader();

    void SetOffset(int offset);
    void SetLoop(bool loop);
    void SetGain(float gain);
    void SetLoader(IAudioSourceLoader* loader);

    void Play();
    void Stop();
    bool IsPlaying();

    void SetPosv3(Point3 value);
    void SetVelocityv3(Vector3 value);
    void SetPosAutoVelv3(Point3 value);
    
    void SetDopplerEffect(bool on);
    bool HasDopplerEffect();
};

#endif