#ifndef __VECMATH__
#define __VECMATH__

#define PI 3.1415926535897932384626433832795

#define Sqrt(x) (__builtin_sqrt(x))
#define Sin(x) (__builtin_sin(x))
#define Cos(x) (__builtin_cos(x))
#define Exp(x) (__builtin_exp(x))
#define Pow(x, y) (__builtin_pow(x, y))
#define Abs(x) (__builtin_fabs(x))
#define Max(x, y) (__builtin_fmax(x, y))
#define Floor(x) (__builtin_floor(x))
#define Ceil(x) (__builtin_ceil(x))
#define Round(x) (__builtin_round(x))
#define SinCos(x, y, z) (__builtin_sincos(x, y, z))
#define SinCosf(x, y, z) (__builtin_sincosf(x, y, z))
#define Asin(x) (__builtin_asin(x))
#define Acos(x) (__builtin_acos(x))
#define Atan(x) (__builtin_atan(x))
#define Sinh(x) (__builtin_sinh(x))
#define Cosh(x) (__builtin_cosh(x))
#define Tanh(x) (__builtin_tanh(x))
#define Asinh(x) (__builtin_asinh(x))
#define Acosh(x) (__builtin_acosh(x))
#define Atanh(x) (__builtin_atanh(x))

#define ToRadian(x) ((x) * 0.017453292519f)
#define ToAngle(x) ((x) * 57.29577951308f)

class Vector2;
class Vector3;
class Quaternion;

class Vector2{
public:
    float x;
    float y;

    static const Vector2 zero;
    static const Vector2 one;
    static const Vector2 up;
    static const Vector2 down;
    static const Vector2 left;
    static const Vector2 right;

    Vector2();
    Vector2(Vector2 &&) = default;
    Vector2(const Vector2 &) = default;
    Vector2 &operator=(Vector2 &&) = default;
    Vector2 &operator=(const Vector2 &) = default;
    ~Vector2();
    Vector2(float, float);
    Vector2 operator+(Vector2) const;
    Vector2 operator-(Vector2) const;
    Vector2 operator*(float) const;
    Vector2 operator/(float) const;
    Vector2 &operator+=(Vector2);
    Vector2 &operator-=(Vector2);
    Vector2 &operator*=(float);
    Vector2 &operator/=(float);
    Vector2 operator-() const;
    Vector2 &operator*=(Quaternion);
    Vector2 &operator/=(Quaternion);
    
    static float Dot(Vector2, Vector2);
    static float Cross(Vector2, Vector2);
    static float Cosine(Vector2, Vector2);
    static float Sine(Vector2, Vector2);
    static float Tangent(Vector2, Vector2);

    Vector2 Normal() const;
    Vector2 &Normalize();
    float Magnitude() const;
    float SqrMagnitude() const;
    Vector2 Reflect(Vector2) const;
    Vector2 Refract(Vector2, float) const;
    Vector2 Rotate(float) const;
};

class Vector3{
public:
    float x;
    float y;
    float z;

    static const Vector3 zero;
    static const Vector3 one;
    static const Vector3 up;
    static const Vector3 down;
    static const Vector3 left;
    static const Vector3 right;
    static const Vector3 forward;
    static const Vector3 back;

    Vector3();
    Vector3(Vector3 &&) = default;
    Vector3(const Vector3 &) = default;
    Vector3 &operator=(Vector3 &&) = default;
    Vector3 &operator=(const Vector3 &) = default;
    ~Vector3();
    Vector3(float, float, float);
    Vector3 operator+(Vector3) const;
    Vector3 operator-(Vector3) const;
    Vector3 operator*(float) const;
    Vector3 operator/(float) const;
    Vector3 &operator+=(Vector3);
    Vector3 &operator-=(Vector3);
    Vector3 &operator*=(float);
    Vector3 &operator/=(float);
    Vector3 operator-() const;
    Vector3 &operator*=(Quaternion);
    Vector3 &operator/=(Quaternion);
    
    static float Dot(Vector3, Vector3);
    static Vector3 Cross(Vector3, Vector3);
    static float Cosine(Vector3, Vector3);
    static float Sine(Vector3, Vector3);
    static float Tangent(Vector3, Vector3);

    Vector3 Normal() const;
    Vector3 &Normalize();
    float Magnitude() const;
    float SqrMagnitude() const;
    Vector3 Reflect(Vector3) const;
    Vector3 Refract(Vector3, float) const;
    Vector3 RotateX(float) const;
    Vector3 RotateY(float) const;
    Vector3 RotateZ(float) const;
};

class Quaternion{
public:
    float x;
    float y;
    float z;
    float w;

    static const Quaternion one;

    Quaternion();
    Quaternion(Quaternion &&) = default;
    Quaternion(const Quaternion &) = default;
    Quaternion &operator=(Quaternion &&) = default;
    Quaternion &operator=(const Quaternion &) = default;
    ~Quaternion();
    Quaternion(float, float, float, float);
    Quaternion(Vector3, float);
    Quaternion operator*(Quaternion) const;
    Quaternion operator*(float) const;
    Quaternion operator/(Quaternion) const;
    Quaternion operator/(float) const;
    Quaternion &operator*=(Quaternion);
    Quaternion &operator*=(float);
    Quaternion &operator/=(Quaternion);
    Quaternion &operator/=(float);
    Quaternion operator-() const;
    Vector3 operator*(Vector3) const;

    static Quaternion FromTo(Vector3, Vector3);
    static Quaternion Reflection(Vector3, Vector3);
    static Quaternion AxisAngle(Vector3, float);
    static Quaternion EulerZXY(float, float, float);
    static Quaternion EulerZXY(Vector3);

    Quaternion Conjugate() const;
    Quaternion Inverse() const;
    Quaternion Normal() const;
    Quaternion &Normalize();
    float Magnitude() const;
    float SqrMagnitude() const;
    Vector3 GetAxis() const;
};

float Lerp(float a, float b, float t);
Vector2 Lerp(Vector2 a, Vector2 b, float t);
Vector3 Lerp(Vector3 a, Vector3 b, float t);
Quaternion Slerp(Quaternion a, Quaternion b, float t);

#endif