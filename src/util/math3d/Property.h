#ifndef __UTILS_MATH3D_PROPERTY__
#define __UTILS_MATH3D_PROPERTY__

#include <define.h>

#include <util/math3d/LinearAlgebra.h>

// 除非管理好引用关系，不然不能直接赋值
class Property : public Object {
private:
    float value = 0.0f;

    AnimationCurve* curve = NULL;

public:
    Property();
    Property(float value);
    Property(float value, bool initCurve);
    ~Property();

    inline float Get(){ return value; }
    inline void Set(float val){ value = val; }

    inline Property& operator=(float val){ value = val; return *this; }
    inline float operator+(float val){ return value + val; }
    inline float operator-(float val){ return value - val; }
    inline float operator*(float val){ return value * val; }
    inline float operator/(float val){ return value / val; }
    inline Property& operator+=(float val){ value += val; return *this; }
    inline Property& operator-=(float val){ value -= val; return *this; }
    inline Property& operator*=(float val){ value *= val; return *this; }
    inline Property& operator/=(float val){ value /= val; return *this; }
    
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
    PropertyVector3(Vector3 value);
    PropertyVector3(Vector3 value, bool initCurve);
    ~PropertyVector3();

    inline Vector3 Get(){ return Vector3(x.Get(), y.Get(), z.Get()); }
    inline void Set(Vector3 val){ x.Set(val.x); y.Set(val.y); z.Set(val.z); }

    inline PropertyVector3& operator=(Vector3 val){ Set(val); return *this; }
    inline Vector3 operator+(Vector3 val){ return Get() + val; }
    inline Vector3 operator-(Vector3 val){ return Get() - val; }
    inline Vector3 operator*(float val){ return Get() * val; }
    inline Vector3 operator/(float val){ return Get() / val; }
    inline Vector3 operator*(Vector3 val){ return Get() * val; }
    inline Vector3 operator/(Vector3 val){ return Get() / val; }
    inline PropertyVector3& operator+=(Vector3 val){ Set(Get() + val); return *this; }
    inline PropertyVector3& operator-=(Vector3 val){ Set(Get() - val); return *this; }
    inline PropertyVector3& operator*=(float val){ Set(Get() * val); return *this; }
    inline PropertyVector3& operator/=(float val){ Set(Get() / val); return *this; }
    inline PropertyVector3& operator*=(Vector3 val){ Set(Get() * val); return *this; }
    inline PropertyVector3& operator/=(Vector3 val){ Set(Get() / val); return *this; }

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
    PropertyQuaternion(Quaternion value);
    PropertyQuaternion(Quaternion value, bool initCurve);
    ~PropertyQuaternion();

    inline Quaternion Get(){ return Quaternion(x.Get(), y.Get(), z.Get(), w.Get()); }
    inline void Set(Quaternion val){ x.Set(val.x); y.Set(val.y); z.Set(val.z); w.Set(val.w); }

    inline PropertyQuaternion& operator=(Quaternion val){ Set(val); return *this; }
    inline Quaternion operator*(Quaternion val){ return Get() * val; }
    inline Quaternion operator/(Quaternion val){ return Get() / val; }
    inline PropertyQuaternion& operator*=(Quaternion val){ Set(Get() * val); return *this; }
    inline PropertyQuaternion& operator/=(Quaternion val){ Set(Get() / val); return *this; }
    inline Vector3 operator*(Vector3 val){ return Get() * val; }

    void SetFrame(float frame);

    void InsertValue(float frame);
    void InsertValue(float frame, Quaternion val);
};

#endif