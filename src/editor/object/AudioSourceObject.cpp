#include <editor/object/AudioSourceObject.h>

#include <openal/al.h>

#include <main.h>
#include <util/gl/GLSimplified.h>
#include <util/AudioUtils.h>
#include <util/os/Time.h>
#include <editor/AudioPlayerWindow.h>

namespace simple3deditor {

CAudioSourceLoader::CAudioSourceLoader(AudioSourceObject*& ref) : ref(ref) {}

CAudioSourceLoader::~CAudioSourceLoader(){
    // 相当于加载NULL
    if (ref)
        ref->SetLoader(NULL);
    ref = NULL;
}

void CAudioSourceLoader::AudioSourceLoad(AudioSourceObject* o){
    if (o){
        if (o->GetLoader()){
            DebugError("CAudioSourceLoader::LoadObject Object Has Already Been Loaded");
            return;
        }
        if (ref)
            ref->SetLoader(NULL);
        ref = o;
        o->SetLoader(this);
    }else{
        if (ref)
            ref->SetLoader(NULL);
        ref = NULL;
    }
}

// 不会在菜单显示，需要通过载入音频文件的方式创建
OBJECT_INFO_DECL(simple3deditor::AudioSourceObject, L"");

static const int REPLAY_ERROR = 4000;

AudioSourceObject::AudioSourceObject() {
    alGenSources(1, &alSrc);
    alGenBuffers(1, &alBuf);

    alSourcef(alSrc, AL_REFERENCE_DISTANCE, 1.0f);
    alSourcef(alSrc, AL_ROLLOFF_FACTOR, 2.0f);
    alSourcef(alSrc, AL_MAX_DISTANCE, 100.0f);

    alSampleSize = 0;
    alChannels = 0;
    alAudioData = 0;
    alAudioSize = 0;
    alAudioLen = 0;
    alAudioFreq = 0;
    alAudioOffset = 0;

    recPos = Point3::zero;
}

AudioSourceObject::AudioSourceObject(uenum format, char* data, int size, int freq) : AViewObject(L"AudioSource", ViewObjectType::OBJECT_AUDIO_SOURCE) {
    alGenSources(1, &alSrc);
    alGenBuffers(1, &alBuf);

    alSourcef(alSrc, AL_REFERENCE_DISTANCE, 1.0f);
    alSourcef(alSrc, AL_ROLLOFF_FACTOR, 2.0f);
    alSourcef(alSrc, AL_MAX_DISTANCE, 100.0f);

    alBufferData(alBuf, format, data, size, freq);
    alSourceQueueBuffers(alSrc, 1, &alBuf);

    switch (format){
    case AL_FORMAT_MONO8:
        alSampleSize = 1;
        alChannels = 1;
        break;
    case AL_FORMAT_MONO16:
        alSampleSize = 2;
        alChannels = 1;
        break;
    case AL_FORMAT_STEREO8:
        alSampleSize = 2;
        alChannels = 2;
        break;
    case AL_FORMAT_STEREO16:
        alSampleSize = 4;
        alChannels = 2;
        break;
    }

    alAudioData = data;
    alAudioSize = size / alSampleSize;
    alAudioLen = alAudioSize / freq;
    alAudioFreq = freq;
    alAudioOffset = 0;

    recPos = Point3::zero;
}

AudioSourceObject::~AudioSourceObject(){
    if (alAudioData) delete[] (char*)alAudioData;

    alDeleteSources(1, &alSrc);
    alDeleteBuffers(1, &alBuf);

    if (loader) loader->AudioSourceLoad(NULL);
}

void AudioSourceObject::OnRender(){
    AViewObject::OnRender();

    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(4.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    Point3 pos = transform.GetWorldTranslation();
    if (dopplerEffect){
        SetPosAutoVelv3(pos);
    }else{
        SetPosv3(pos);
    }
}

char* AudioSourceObject::GetData(){
    return (char*)alAudioData;
}

int AudioSourceObject::GetSize(){
    return alAudioSize;
}

int AudioSourceObject::GetLength(){
    return alAudioLen;
}

int AudioSourceObject::GetSampleSize(){
    return alSampleSize;
}

int AudioSourceObject::GetChannelCount(){
    return alChannels;
}

int AudioSourceObject::GetFrequency(){
    return alAudioFreq;
}

int AudioSourceObject::GetOffset(){
    if (IsPlaying())
        alGetSourcei(alSrc, AL_SAMPLE_OFFSET, &alAudioOffset);
    return alAudioOffset;
}

bool AudioSourceObject::IsLoop(){
    ALint loop;
    alGetSourcei(alSrc, AL_LOOPING, &loop);
    return loop == AL_TRUE;
}

float AudioSourceObject::GetGain(){
    float gain;
    alGetSourcef(alSrc, AL_GAIN, &gain);
    return gain;
}

IAudioSourceLoader* AudioSourceObject::GetLoader(){
    return loader;
}

void AudioSourceObject::SetOffset(int offset){
    if (IsPlaying())
        alSourcei(alSrc, AL_SAMPLE_OFFSET, offset);
    alAudioOffset = offset;
}

void AudioSourceObject::SetLoop(bool loop){
    alSourcei(alSrc, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void AudioSourceObject::SetGain(float gain){
    alSourcef(alSrc, AL_GAIN, gain);
}

void AudioSourceObject::SetLoader(IAudioSourceLoader* loader){
    this->loader = loader;
}

void AudioSourceObject::Play(){
    alSourcePlay(alSrc);
    if (alAudioOffset + REPLAY_ERROR >= alAudioSize)
        alAudioOffset = 0;
    alSourcei(alSrc, AL_SAMPLE_OFFSET, alAudioOffset);
}

void AudioSourceObject::Stop(){
    alGetSourcei(alSrc, AL_SAMPLE_OFFSET, &alAudioOffset);
    alSourceStop(alSrc);
}

bool AudioSourceObject::IsPlaying(){
    ALint state;
    alGetSourcei(alSrc, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void AudioSourceObject::SetPosv3(Point3 value){
    alSource3f(alSrc, AL_POSITION, value.x, value.y, value.z);
}

void AudioSourceObject::SetVelocityv3(Vector3 value){
    alSource3f(alSrc, AL_VELOCITY, value.x, value.y, value.z);
}

// 如果是渲染过程中调用则可用Time::GetDeltaTime()
void AudioSourceObject::SetPosAutoVelv3(Point3 value){
    Vector3 vel = (value - recPos) / Time::GetDeltaTime();
    recPos = value;

    alSource3f(alSrc, AL_POSITION, value.x, value.y, value.z);
    alSource3f(alSrc, AL_VELOCITY, vel.x, vel.y, vel.z);
}

void AudioSourceObject::SetDopplerEffect(bool on){
    if (dopplerEffect == on)
        return;
    dopplerEffect = on;
    if (on){
        recPos = transform.GetWorldTranslation();
    }else{
        SetVelocityv3(Vector3::zero);
    }
}

bool AudioSourceObject::HasDopplerEffect(){
    return dopplerEffect;
}

}