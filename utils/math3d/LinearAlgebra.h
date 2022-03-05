#ifndef __UTILS_MATH3D_LINEARALGEBRA__
#define __UTILS_MATH3D_LINEARALGEBRA__

#include <define.h>

class Vector2 final : public Object {
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

class Vector3 final : public Object {
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
    static Vector3 Compose(Vector3 coord, Vector3 x, Vector3 y, Vector3 z);
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
class Vector4 final : public Object {
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

// 复数在二维上有与四元数在三维上相似的功能，即表示一个旋转
// 不同的是复数满足交换律，而四元数不满足
class Complex final : public Object {
public:
    union {
        _Complex float comp;
        struct {
            float real;
            float imag;
        };
    };

    static const Complex one;
    static const Complex i;

    Complex();
    Complex(float &&);
    Complex(_Complex float &&);
    Complex(Vector2 &&);
    Complex(Complex &&) = default;
    Complex(const float &);
    Complex(const _Complex float &);
    Complex(const Vector2 &);
    Complex(const Complex &) = default;
    Complex &operator=(float &&);
    Complex &operator=(_Complex float &&);
    Complex &operator=(Vector2 &&);
    Complex &operator=(Complex &&) = default;
    Complex &operator=(const float &);
    Complex &operator=(const _Complex float &);
    Complex &operator=(const Vector2 &);
    Complex &operator=(const Complex &) = default;
    Complex(float, float);
    ~Complex();
    Complex operator+(Complex) const;
    Complex operator+(float) const;
    Complex operator-(Complex) const;
    Complex operator-(float) const;
    Complex operator*(Complex) const;
    Complex operator*(float) const;
    Complex operator/(Complex) const;
    Complex operator/(float) const;
    Complex &operator+=(Complex);
    Complex &operator+=(float);
    Complex &operator-=(Complex);
    Complex &operator-=(float);
    Complex &operator*=(Complex);
    Complex &operator*=(float);
    Complex &operator/=(Complex);
    Complex &operator/=(float);
    Complex operator-() const;
    Vector2 operator*(Vector2) const;

    static Complex FromTo(Vector2 from, Vector2 to);
    static Complex Reflection(Vector2 axis, Vector2 v);
    static Complex Rotation(float);

    Complex Conjugate() const;
    Complex Inverse() const;
    Complex Normal() const;
    Complex &Normalize();
    Complex &Rotate(float);
    float Magnitude() const;
    float SqrMagnitude() const;
};

// 四元数不满足交换律，应按照旋转顺序依次从右往左排列，对向量操作应为左乘
// 四元数的旋转轴即为(x,y,z)方向
class Quaternion final : public Object {
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

    static Quaternion FromTo(Vector3 from, Vector3 to);
    static Quaternion Reflection(Vector3 axis, Vector3 v);
    static Quaternion AxisAngle(Vector3, float);
    static Quaternion EulerZXY(float x, float y, float z);
    static Quaternion EulerZXY(Vector3);
    static Quaternion LookAt(Vector3 dir, Vector3 up);

    Quaternion Conjugate() const;
    Quaternion Inverse() const;
    Quaternion Normal() const;
    Quaternion &Normalize();
    Quaternion &Rotate(Vector3, float);
    float Magnitude() const;
    float SqrMagnitude() const;
    Vector3 GetAxis() const;
};

class Matrix4x4 final : public Object {
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

#endif