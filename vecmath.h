#ifndef __VECMATH__
#define __VECMATH__

#include "define.h"

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
class Vector4;
class Quaternion;
class Matrix4x4;

class Vector2 : public Object {
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
    Vector2(Vector3 &&);
    Vector2(Vector4 &&);
    Vector2(const Vector2 &) = default;
    Vector2(const Vector3 &);
    Vector2(const Vector4 &);
    Vector2 &operator=(Vector2 &&) = default;
    Vector2 &operator=(Vector3 &&);
    Vector2 &operator=(Vector4 &&);
    Vector2 &operator=(const Vector2 &) = default;
    Vector2 &operator=(const Vector3 &);
    Vector2 &operator=(const Vector4 &);
    Vector2(float, float);
    ~Vector2();
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

class Vector3 : public Object {
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
    Vector3(Vector2 &&);
    Vector3(Vector3 &&) = default;
    Vector3(Vector4 &&);
    Vector3(const Vector2 &);
    Vector3(const Vector3 &) = default;
    Vector3(const Vector4 &);
    Vector3 &operator=(Vector2 &&);
    Vector3 &operator=(Vector3 &&) = default;
    Vector3 &operator=(Vector4 &&);
    Vector3 &operator=(const Vector2 &);
    Vector3 &operator=(const Vector3 &) = default;
    Vector3 &operator=(const Vector4 &);
    Vector3(float, float);
    Vector3(float, float, float);
    ~Vector3();
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
    static float Determinant(Vector3, Vector3, Vector3);
    static Vector3 Decompose(Vector3 vec, Vector3 x, Vector3 y, Vector3 z);

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

// 齐次三维坐标向量
class Vector4 : public Object {
public:
    float x;
    float y;
    float z;
    // 第四维一般不会使用
    // 除了加减操作中，点的第四维为1.0，向量第四维为0.0
    // 此时它们相互进行运算
    // 以及在四维矩阵变换中
    // 第四维为1.0的点会参与平移运算
    // 第四维为0.0的向量不会
    float w;

    static const Vector4 zero;
    static const Vector4 one;
    static const Vector4 up;
    static const Vector4 down;
    static const Vector4 left;
    static const Vector4 right;
    static const Vector4 forward;
    static const Vector4 back;

    Vector4();
    Vector4(Vector2 &&);
    Vector4(Vector3 &&);
    Vector4(Vector4 &&) = default;
    Vector4(const Vector2 &);
    Vector4(const Vector3 &);
    Vector4(const Vector4 &) = default;
    Vector4 &operator=(Vector2 &&);
    Vector4 &operator=(Vector3 &&);
    Vector4 &operator=(Vector4 &&) = default;
    Vector4 &operator=(const Vector2 &);
    Vector4 &operator=(const Vector3 &);
    Vector4 &operator=(const Vector4 &) = default;
    Vector4(float, float);
    Vector4(float, float, float);
    Vector4(float, float, float, float);
    Vector4(Vector3, float);
    ~Vector4();
    Vector4 operator+(Vector3) const;
    Vector4 operator+(Vector4) const;
    Vector4 operator-(Vector3) const;
    Vector4 operator-(Vector4) const;
    Vector4 operator*(float) const;
    Vector4 operator/(float) const;
    Vector4 &operator+=(Vector3);
    Vector4 &operator-=(Vector3);
    Vector4 &operator*=(float);
    Vector4 &operator/=(float);
    Vector4 operator-() const;
    Vector4 &operator*=(Quaternion);
    Vector4 &operator/=(Quaternion);

    static float Dot(Vector4, Vector4);
    static Vector4 Cross(Vector4, Vector4);
    static float Cosine(Vector4, Vector4);
    static float Sine(Vector4, Vector4);
    static float Tangent(Vector4, Vector4);

    Vector3 Normal() const;
    Vector4 &Normalize();
    float Magnitude() const;
    float SqrMagnitude() const;
    Vector4 Reflect(Vector3) const;
    Vector4 Refract(Vector3, float) const;
    Vector4 RotateX(float) const;
    Vector4 RotateY(float) const;
    Vector4 RotateZ(float) const;
};

class Quaternion : public Object {
public:
    // 四元数的旋转轴即为(x,y,z)方向
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
    Quaternion(float, float, float, float);
    Quaternion(Vector3, float);
    ~Quaternion();
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
    Quaternion &Rotate(Vector3, float);
    float Magnitude() const;
    float SqrMagnitude() const;
    Vector3 GetAxis() const;
};

class Matrix4x4 : public Object {
public:
    // OpenGL为列优先顺序
    // 此矩阵变量命名使用行优先顺序
    float _11;
    float _21;
    float _31;
    float _41;

    float _12;
    float _22;
    float _32;
    float _42;

    float _13;
    float _23;
    float _33;
    float _43;

    float _14;
    float _24;
    float _34;
    float _44;

    static const Matrix4x4 zero;
    static const Matrix4x4 identity;

    Matrix4x4();
    Matrix4x4(Quaternion &&);
    Matrix4x4(Matrix4x4 &&) = default;
    Matrix4x4(const Quaternion &);
    Matrix4x4(const Matrix4x4 &) = default;
    Matrix4x4 &operator=(Quaternion &&);
    Matrix4x4 &operator=(Matrix4x4 &&) = default;
    Matrix4x4 &operator=(const Quaternion &);
    Matrix4x4 &operator=(const Matrix4x4 &) = default;
    Matrix4x4(float, float, float, float,
              float, float, float, float,
              float, float, float, float,
              float, float, float, float);
    ~Matrix4x4();
    Matrix4x4 operator+(Matrix4x4) const;
    Matrix4x4 operator-(Matrix4x4) const;
    Matrix4x4 operator*(float) const;
    Matrix4x4 operator/(float) const;
    Matrix4x4 &operator+=(Matrix4x4);
    Matrix4x4 &operator-=(Matrix4x4);
    Matrix4x4 &operator*=(float);
    Matrix4x4 &operator/=(float);
    Matrix4x4 operator*(Matrix4x4) const;
    Matrix4x4 &operator*=(Matrix4x4);
    Matrix4x4 operator-() const;
    Matrix4x4 operator~() const;
    Vector3 operator*(Vector3) const;
    Vector4 operator*(Vector4) const;
};

float GetRate(float x, float a, float b);

float Lerp(float a, float b, float t);
Vector2 Lerp(Vector2 a, Vector2 b, float t);
Vector3 Lerp(Vector3 a, Vector3 b, float t);
Quaternion Slerp(Quaternion a, Quaternion b, float t);

int Clamp(int x, int a, int b);
float Clamp(float x, float a, float b);

void Sort(float& x, float& y);

#endif