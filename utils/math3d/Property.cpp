#include <utils/math3d/Property.h>

#include <editor/gui/AnimationCurve.h>
#include <utils/math3d/LinearAlgebra.h>

Property::Property(){}
Property::Property(float value, bool initCurve) : value(value), curve(initCurve ? new AnimationCurve(0.0f, 250.0f) : NULL) {}

Property::~Property(){
    if (curve) delete curve;
}

float Property::Get(){
    return value;
}

void Property::Set(float val){
    value = val;
}
    
void Property::SetCurve(AnimationCurve* curve){
    if (this->curve)
        delete this->curve;
    this->curve = curve;
}

AnimationCurve* Property::GetCurve(){
    return curve;
}

AnimationCurve* Property::GenCurve(){
    if (!curve)
        curve = new AnimationCurve(0.0f, 250.0f);
    return curve;
}

void Property::SetFrame(float frame){
    if (curve)
        value = curve->GetValue(frame);
}

void Property::InsertValue(float frame){
    GenCurve()->AddPoint(Vector2(frame, value));
}

void Property::InsertValue(float frame, float val){
    GenCurve()->AddPoint(Vector2(frame, val));
}

PropertyVector3::PropertyVector3(){}
PropertyVector3::PropertyVector3(Vector3 value, bool initCurve) : x(value.x, initCurve), y(value.y, initCurve), z(value.z, initCurve) {}
PropertyVector3::~PropertyVector3(){}

Vector3 PropertyVector3::Get(){
    return Vector3(x.Get(), y.Get(), z.Get());
}

void PropertyVector3::Set(Vector3 val){
    x.Set(val.x); y.Set(val.y); z.Set(val.z);
}

void PropertyVector3::SetFrame(float frame){
    x.SetFrame(frame); y.SetFrame(frame); z.SetFrame(frame);
}

void PropertyVector3::InsertValue(float frame){
    x.InsertValue(frame); y.InsertValue(frame); z.InsertValue(frame);
}

void PropertyVector3::InsertValue(float frame, Vector3 val){
    x.InsertValue(frame, val.x); y.InsertValue(frame, val.y); z.InsertValue(frame, val.z);
}

PropertyQuaternion::PropertyQuaternion(){}
PropertyQuaternion::PropertyQuaternion(Quaternion value, bool initCurve) : x(value.x, initCurve), y(value.y, initCurve), z(value.z, initCurve), w(value.w, initCurve) {}
PropertyQuaternion::~PropertyQuaternion(){}

Quaternion PropertyQuaternion::Get(){
    return Quaternion(x.Get(), y.Get(), z.Get(), w.Get());
}

void PropertyQuaternion::Set(Quaternion val){
    x.Set(val.x); y.Set(val.y); z.Set(val.z); w.Set(val.w);
}

void PropertyQuaternion::SetFrame(float frame){
    x.SetFrame(frame); y.SetFrame(frame); z.SetFrame(frame); w.SetFrame(frame);
}

void PropertyQuaternion::InsertValue(float frame){
    x.InsertValue(frame); y.InsertValue(frame); z.InsertValue(frame); w.InsertValue(frame);
}

void PropertyQuaternion::InsertValue(float frame, Quaternion val){
    x.InsertValue(frame, val.x); y.InsertValue(frame, val.y); z.InsertValue(frame, val.z); w.InsertValue(frame, val.w);
}