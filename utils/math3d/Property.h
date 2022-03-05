#ifndef __UTILS_MATH3D_PROPERTY__
#define __UTILS_MATH3D_PROPERTY__

#include <define.h>

// 除非管理好引用关系，不然不能直接赋值
class Property : public Object {
private:
    float value = 0.0f;

    AnimationCurve* curve = NULL;

public:
    Property();
    Property(float value, bool initCurve = false);
    ~Property();

    float Get();
    void Set(float val);
    
    void SetCurve(AnimationCurve* curve);
    AnimationCurve* GetCurve();
    // 不存在曲线时创建
    AnimationCurve* GenCurve();
    void SetFrame(float frame);
    void InsertValue(float frame);
    void InsertValue(float frame, float val);
};

class PropertyVector3 : public Object {
public:
    Property x;
    Property y;
    Property z;

    PropertyVector3();
    PropertyVector3(Vector3 value, bool initCurve = false);
    ~PropertyVector3();

    Vector3 Get();
    void Set(Vector3 val);

    void SetFrame(float frame);

    void InsertValue(float frame);
    void InsertValue(float frame, Vector3 val);
};

class PropertyQuaternion : public Object {
public:
    Property x;
    Property y;
    Property z;
    Property w;

    PropertyQuaternion();
    PropertyQuaternion(Quaternion value, bool initCurve = false);
    ~PropertyQuaternion();

    Quaternion Get();
    void Set(Quaternion val);

    void SetFrame(float frame);

    void InsertValue(float frame);
    void InsertValue(float frame, Quaternion val);
};

#endif