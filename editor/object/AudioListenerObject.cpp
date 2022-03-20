#include <editor/object/AudioListenerObject.h>

#include <lib/openal/al.h>

#include <main.h>
#include <utils/gl/GLSimplified.h>
#include <utils/gl/GLUtils.h>
#include <utils/AudioUtils.h>
#include <utils/os/Time.h>

AudioListenerObject::AudioListenerObject() : AViewObject(L"AudioListener", ViewObjectType::OBJECT_AUDIO_LISTENER) {
    recPos = Vector3::zero;
    recTime = TimeUtils::GetTime();
}

AudioListenerObject::~AudioListenerObject(){
    LocalData::GetLocalInst()->audioListener = NULL;
}

void AudioListenerObject::OnRender(){
    AViewObject::OnRender();

    Vector3 pos = transform.chainMat * Point3::zero;
    Vector3 dir = (transform.chainMat * Vector3::forward).Normal();
    Vector3 up = (transform.chainMat * Vector3::up).Normal();

    alListenerPosv3(pos);
    alListenerDirv3(dir, up);

    glColor3f(1.0f, 0.0f, 0.0f);
    GLUtils::Draw3DArrow(Point3::zero, Vector3::forward, 0.3f, 0.3f, 10.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    GLUtils::Draw3DArrow(Point3::zero, Vector3::up * 0.5f, 0.08f, 0.3f, 5.0f);
}

void AudioListenerObject::SetPosv3(Point3 value){
    alListener3f(AL_POSITION, value.x, value.y, value.z);
}

void AudioListenerObject::SetVelocityv3(Vector3 value){
    alListener3f(AL_VELOCITY, value.x, value.y, value.z);
}

void AudioListenerObject::SetPosAutoVelv3(Point3 value){
    float curTime = TimeUtils::GetTime();
    Vector3 vel = (value - recPos) / (curTime - recTime);

    recPos = value;
    recTime = curTime;

    alListener3f(AL_POSITION, value.x, value.y, value.z);
    alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
}

void AudioListenerObject::SetDopplerEffect(bool on){
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

bool AudioListenerObject::HasDopplerEffect(){
    return dopplerEffect;
}