#include <editor/object/AudioListenerObject.h>

#include <lib/openal/al.h>

#include <main.h>
#include <util/gl/GLSimplified.h>
#include <util/gl/GLUtils.h>
#include <util/AudioUtils.h>
#include <util/os/Time.h>

AudioListenerObject::AudioListenerObject() : AViewObject(L"AudioListener", ViewObjectType::OBJECT_AUDIO_LISTENER) {
    recPos = Point3::zero;
}

AudioListenerObject::~AudioListenerObject(){
    LocalData::GetLocalInst()->audioListener = NULL;
}

void AudioListenerObject::OnRender(){
    AViewObject::OnRender();

    Vector3 pos = transform.GetWorldTranslation();
    Vector3 dir = (transform.chainMat * Vector3::forward).Normal();
    Vector3 up = (transform.chainMat * Vector3::up).Normal();

    if (dopplerEffect){
        SetPosAutoVelv3(pos);
    }else{
        SetPosv3(pos);
    }
    SetDirv3(dir, up);

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

// 如果是渲染过程中调用则可用Time::GetDeltaTime()
void AudioListenerObject::SetPosAutoVelv3(Point3 value){
    Vector3 vel = (value - recPos) / Time::GetDeltaTime();
    recPos = value;

    alListener3f(AL_POSITION, value.x, value.y, value.z);
    alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
}

void AudioListenerObject::SetDirv3(Vector3 dir, Vector3 up){
    float ori[] = {dir.x, dir.y, dir.z, up.x, up.y, up.z};
    alListenerfv(AL_ORIENTATION, ori);
}

void AudioListenerObject::SetDopplerEffect(bool on){
    if (dopplerEffect == on)
        return;
    dopplerEffect = on;
    if (on){
        recPos = transform.GetWorldTranslation();
    }else{
        SetVelocityv3(Vector3::zero);
    }
}

bool AudioListenerObject::HasDopplerEffect(){
    return dopplerEffect;
}