#include <editor/object/AudioSourceObject.h>

#include <lib/openal/al.h>

#include <main.h>
#include <utils/gl/GLSimplified.h>
#include <utils/AudioUtils.h>
#include <utils/os/Time.h>
#include <editor/AudioPlayerWindow.h>

static const int REPLAY_ERROR = 4000;

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

    recPos = Vector3::zero;
    recTime = TimeUtils::GetTime();
}

AudioSourceObject::~AudioSourceObject(){
    if (alAudioData) delete[] (char*)alAudioData;

    alDeleteSources(1, &alSrc);
    alDeleteBuffers(1, &alBuf);

    if (window) window->LoadObject(NULL);
}

void AudioSourceObject::OnTimer(int id){
    Point3 pos = transform.chainMat * Point3::zero;
    if (dopplerEffect){
        SetPosAutoVelv3(pos);
    }else{
        SetPosv3(pos);
    }
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

AudioPlayerWindow* AudioSourceObject::GetWindowRef(){
    return window;
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

void AudioSourceObject::SetWindowRef(AudioPlayerWindow* window){
    this->window = window;
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

void AudioSourceObject::SetPosAutoVelv3(Point3 value){
    float curTime = TimeUtils::GetTime();
    Vector3 vel = (value - recPos) / (curTime - recTime);

    recPos = value;
    recTime = curTime;

    alSource3f(alSrc, AL_POSITION, value.x, value.y, value.z);
    alSource3f(alSrc, AL_VELOCITY, vel.x, vel.y, vel.z);
}

void AudioSourceObject::SetDopplerEffect(bool on){
    if (dopplerEffect == on)
        return;
    dopplerEffect = on;
    if (on){
        recPos = transform.chainMat * Point3::zero;
        recTime = TimeUtils::GetTime();
    }else{
        SetVelocityv3(Vector3::zero);
    }
}

bool AudioSourceObject::HasDopplerEffect(){
    return dopplerEffect;
}