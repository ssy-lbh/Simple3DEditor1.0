#ifndef __UTIL_MATH3D_LINEARALGEBRA__
#define __UTIL_MATH3D_LINEARALGEBRA__

#include <define.h>

#include <util/math3d/Math.h>

namespace simple3deditor {

template <typename T>
class TVector2 {
public:
    T x;
    T y;

    static const TVector2<T> zero;
    static const TVector2<T> one;
    static const TVector2<T> up;
    static const TVector2<T> down;
    static const TVector2<T> left;
    static const TVector2<T> right;

    TVector2<T>();
    TVector2<T>(TVector2<T> &&) = default;
    TVector2<T>(TVector3<T> &&);
    TVector2<T>(TVector4<T> &&);
    TVector2<T>(const TVector2<T> &) = default;
    TVector2<T>(const TVector3<T> &);
    TVector2<T>(const TVector4<T> &);
    TVector2<T> &operator=(TVector2<T> &&) = default;
    TVector2<T> &operator=(TVector3<T> &&);
    TVector2<T> &operator=(TVector4<T> &&);
    TVector2<T> &operator=(const TVector2<T> &) = default;
    TVector2<T> &operator=(const TVector3<T> &);
    TVector2<T> &operator=(const TVector4<T> &);
    constexpr TVector2<T>(T, T);
    TVector2<T> operator+(const TVector2<T>&) const;
    TVector2<T> operator-(const TVector2<T>&) const;
    TVector2<T> operator/(T) const;
    TVector2<T> operator*(const TVector2<T>&) const;
    TVector2<T> operator/(const TVector2<T>&) const;
    TVector2<T> &operator+=(const TVector2<T>&);
    TVector2<T> &operator-=(const TVector2<T>&);
    TVector2<T> &operator*=(T);
    TVector2<T> &operator/=(T);
    TVector2<T> operator-() const;
    TVector2<T> &operator*=(const TQuaternion<T>&);
    TVector2<T> &operator/=(const TQuaternion<T>&);
    
    static T Dot(const TVector2<T>&, const TVector2<T>&);
    static T Cross(const TVector2<T>&, const TVector2<T>&);
    static T Cosine(const TVector2<T>&, const TVector2<T>&);
    static T Sine(const TVector2<T>&, const TVector2<T>&);
    static T Tangent(const TVector2<T>&, const TVector2<T>&);

    TVector2<T> Normal() const;
    TVector2<T> &Normalize();
    T Magnitude() const;
    T SqrMagnitude() const;
    TVector2<T> Reflect(TVector2<T>) const;
    TVector2<T> Refract(TVector2<T>, T) const;
    TVector2<T> Rotate(T) const;
};

template <typename T>
class TPoint2 : public TVector2<T> {
public:
    static const TPoint2<T> zero;

    TPoint2<T>();
    TPoint2<T>(TVector2<T> &&);
    TPoint2<T>(TPoint2<T> &&) = default;
    TPoint2<T>(const TVector2<T> &);
    TPoint2<T>(const TPoint2<T> &) = default;
    TPoint2<T> &operator=(TVector2<T> &&);
    TPoint2<T> &operator=(TPoint2<T> &&) = default;
    TPoint2<T> &operator=(const TVector2<T> &);
    TPoint2<T> &operator=(const TPoint2<T> &) = default;
    constexpr TPoint2<T>(T, T);
};

template <typename T>
class TVector3 {
public:
    T x;
    T y;
    T z;

    static const TVector3<T> zero;
    static const TVector3<T> one;
    static const TVector3<T> up;
    static const TVector3<T> down;
    static const TVector3<T> left;
    static const TVector3<T> right;
    static const TVector3<T> forward;
    static const TVector3<T> back;

    TVector3<T>();
    TVector3<T>(TVector2<T> &&);
    TVector3<T>(TVector3<T> &&) = default;
    TVector3<T>(TVector4<T> &&);
    TVector3<T>(const TVector2<T> &);
    TVector3<T>(const TVector3<T> &) = default;
    TVector3<T>(const TVector4<T> &);
    TVector3<T> &operator=(TVector2<T> &&);
    TVector3<T> &operator=(TVector3<T> &&) = default;
    TVector3<T> &operator=(TVector4<T> &&);
    TVector3<T> &operator=(const TVector2<T> &);
    TVector3<T> &operator=(const TVector3<T> &) = default;
    TVector3<T> &operator=(const TVector4<T> &);
    constexpr TVector3<T>(T, T);
    constexpr TVector3<T>(T, T, T);
    TVector3<T> operator+(const TVector3<T>&) const;
    TVector3<T> operator-(const TVector3<T>&) const;
    TVector3<T> operator/(T) const;
    TVector3<T> operator*(const TVector3<T>&) const;
    TVector3<T> operator/(const TVector3<T>&) const;
    TVector3<T> &operator+=(const TVector3<T>&);
    TVector3<T> &operator-=(const TVector3<T>&);
    TVector3<T> &operator*=(T);
    TVector3<T> &operator/=(T);
    TVector3<T> operator-() const;
    TVector3<T> &operator*=(const TQuaternion<T>&);
    TVector3<T> &operator/=(const TQuaternion<T>&);
    
    static T Dot(const TVector3<T>&, const TVector3<T>&);
    static TVector3<T> Cross(const TVector3<T>&, const TVector3<T>&);
    static T Cosine(const TVector3<T>&, const TVector3<T>&);
    static T Sine(const TVector3<T>&, const TVector3<T>&);
    static T Tangent(const TVector3<T>&, const TVector3<T>&);
    static T Determinant(const TVector3<T>&, const TVector3<T>&, const TVector3<T>&);
    static TVector3<T> Compose(const TVector3<T>& coord, const TVector3<T>& x, const TVector3<T>& y, const TVector3<T>& z);
    static TVector3<T> Decompose(const TVector3<T>& vec, const TVector3<T>& x, const TVector3<T>& y, const TVector3<T>& z);
    // 主要是为了可以在调用之前做行列式值是否为零的检查
    static TVector3<T> Decompose(const TVector3<T>& vec, const TVector3<T>& x, const TVector3<T>& y, const TVector3<T>& z, T det);

    TVector3<T> Normal() const;
    TVector3<T> &Normalize();
    T Magnitude() const;
    T SqrMagnitude() const;
    TVector3<T> Reflect(TVector3<T>) const;
    TVector3<T> Refract(TVector3<T>, T) const;
    TVector3<T> RotateX(T) const;
    TVector3<T> RotateY(T) const;
    TVector3<T> RotateZ(T) const;
    TVector2<T> XY() const;
    TVector2<T> YZ() const;
    TVector2<T> ZX() const;
    TVector2<T> YX() const;
    TVector2<T> ZY() const;
    TVector2<T> XZ() const;
};

template <typename T>
class TPoint3 : public TVector3<T> {
public:
    static const TPoint3<T> zero;

    TPoint3<T>();
    TPoint3<T>(TVector3<T> &&);
    TPoint3<T>(TPoint3<T> &&) = default;
    TPoint3<T>(const TVector3<T> &);
    TPoint3<T>(const TPoint3<T> &) = default;
    TPoint3<T> &operator=(TVector3<T> &&);
    TPoint3<T> &operator=(TPoint3<T> &&) = default;
    TPoint3<T> &operator=(const TVector3<T> &);
    TPoint3<T> &operator=(const TPoint3<T> &) = default;
    constexpr TPoint3<T>(T, T);
    constexpr TPoint3<T>(T, T, T);
};

// 齐次三维坐标向量
template <typename T>
class TVector4 {
public:
    T x;
    T y;
    T z;
    // 第四维一般不会使用
    // 除了加减操作中，点的第四维为1.0，向量第四维为0.0.0f
    // 此时它们相互进行运算
    // 以及在四维矩阵变换中
    // 第四维为1.0的点会参与平移运算
    // 第四维为0.0的向量不会
    T w;

    static const TVector4<T> zero;
    static const TVector4<T> one;
    static const TVector4<T> up;
    static const TVector4<T> down;
    static const TVector4<T> left;
    static const TVector4<T> right;
    static const TVector4<T> forward;
    static const TVector4<T> back;

    TVector4<T>();
    TVector4<T>(TVector2<T> &&);
    TVector4<T>(TPoint2<T> &&);
    TVector4<T>(TVector3<T> &&);
    TVector4<T>(TPoint3<T> &&);
    TVector4<T>(TVector4<T> &&) = default;
    TVector4<T>(const TVector2<T> &);
    TVector4<T>(const TPoint2<T> &);
    TVector4<T>(const TVector3<T> &);
    TVector4<T>(const TPoint3<T> &);
    TVector4<T>(const TVector4<T> &) = default;
    TVector4<T> &operator=(TVector2<T> &&);
    TVector4<T> &operator=(TPoint2<T> &&);
    TVector4<T> &operator=(TVector3<T> &&);
    TVector4<T> &operator=(TPoint3<T> &&);
    TVector4<T> &operator=(TVector4<T> &&) = default;
    TVector4<T> &operator=(const TVector2<T> &);
    TVector4<T> &operator=(const TPoint2<T> &);
    TVector4<T> &operator=(const TVector3<T> &);
    TVector4<T> &operator=(const TPoint3<T> &);
    TVector4<T> &operator=(const TVector4<T> &) = default;
    constexpr TVector4<T>(T, T);
    constexpr TVector4<T>(T, T, T);
    constexpr TVector4<T>(T, T, T, T);
    TVector4<T>(const TVector3<T>&, T);
    TVector4<T> operator+(const TVector3<T>&) const;
    TVector4<T> operator+(const TVector4<T>&) const;
    TVector4<T> operator-(const TVector3<T>&) const;
    TVector4<T> operator-(const TVector4<T>&) const;
    TVector4<T> operator*(T) const;
    TVector4<T> operator/(T) const;
    TVector4<T> &operator+=(const TVector3<T>&);
    TVector4<T> &operator-=(const TVector3<T>&);
    TVector4<T> &operator*=(T);
    TVector4<T> &operator/=(T);
    TVector4<T> operator-() const;
    TVector4<T> &operator*=(const TQuaternion<T>&);
    TVector4<T> &operator/=(const TQuaternion<T>&);

    static T Dot(const TVector4<T>&, const TVector4<T>&);
    static TVector4<T> Cross(const TVector4<T>&, const TVector4<T>&);
    static T Cosine(const TVector4<T>&, const TVector4<T>&);
    static T Sine(const TVector4<T>&, const TVector4<T>&);
    static T Tangent(const TVector4<T>&, const TVector4<T>&);

    TVector3<T> Normal() const;
    TVector4<T> &Normalize();
    T Magnitude() const;
    T SqrMagnitude() const;
    TVector4<T> Reflect(TVector3<T>) const;
    TVector4<T> Refract(TVector3<T>, T) const;
    TVector4<T> RotateX(T) const;
    TVector4<T> RotateY(T) const;
    TVector4<T> RotateZ(T) const;
};

// 复数在二维上有与四元数在三维上相似的功能，即表示一个旋转
// 不同的是复数满足交换律，而四元数不满足
// X对应实部，Y对应虚部
template <typename T>
class TComplex {
public:
    T real;
    T imag;

    static const TComplex<T> one;
    static const TComplex<T> i;

    constexpr TComplex<T>();
    TComplex<T>(T &&);
    TComplex<T>(TVector2<T> &&);
    TComplex<T>(TComplex<T> &&) = default;
    TComplex<T>(const T &);
    TComplex<T>(const TVector2<T> &);
    TComplex<T>(const TComplex<T> &) = default;
    TComplex<T> &operator=(T &&);
    TComplex<T> &operator=(TVector2<T> &&);
    TComplex<T> &operator=(TComplex<T> &&) = default;
    TComplex<T> &operator=(const T &);
    TComplex<T> &operator=(const TVector2<T> &);
    TComplex<T> &operator=(const TComplex<T> &) = default;
    constexpr TComplex<T>(T, T);
    TComplex<T> operator+(const TComplex<T>&) const;
    TComplex<T> operator-(const TComplex<T>&) const;
    TComplex<T> operator*(const TComplex<T>&) const;
    TComplex<T> operator/(const TComplex<T>&) const;
    TComplex<T> &operator+=(const TComplex<T>&);
    TComplex<T> &operator+=(T);
    TComplex<T> &operator-=(const TComplex<T>&);
    TComplex<T> &operator-=(T);
    TComplex<T> &operator*=(const TComplex<T>&);
    TComplex<T> &operator*=(T);
    TComplex<T> &operator/=(const TComplex<T>&);
    TComplex<T> &operator/=(T);
    TComplex<T> operator-() const;
    TVector2<T> operator*(const TVector2<T>&) const;

    static TComplex<T> FromTo(TVector2<T> from, TVector2<T> to);
    static TComplex<T> Reflection(TVector2<T> axis, TVector2<T> v);
    static TComplex<T> Rotation(T);

    TComplex<T> Conjugate() const;
    TComplex<T> Inverse() const;
    TComplex<T> Normal() const;
    TComplex<T> &Normalize();
    TComplex<T> &Rotate(T);
    T Magnitude() const;
    T SqrMagnitude() const;
};

// 四元数不满足交换律，应按照旋转顺序依次从右往左排列，对向量操作应为左乘
// 四元数的旋转轴即为(x,y,z)方向
template <typename T>
class TQuaternion {
public:
    T x;
    T y;
    T z;
    T w;

    static const TQuaternion<T> one;

    constexpr TQuaternion<T>();
    TQuaternion<T>(TQuaternion<T> &&) = default;
    TQuaternion<T>(const TQuaternion<T> &) = default;
    TQuaternion<T> &operator=(TQuaternion<T> &&) = default;
    TQuaternion<T> &operator=(const TQuaternion<T> &) = default;
    constexpr TQuaternion<T>(T, T, T, T);
    TQuaternion<T>(const TVector3<T>&, T);
    TQuaternion<T> operator*(const TQuaternion<T>&) const;
    TQuaternion<T> operator*(T) const;
    TQuaternion<T> operator/(const TQuaternion<T>&) const;
    TQuaternion<T> operator/(T) const;
    TQuaternion<T> &operator*=(const TQuaternion<T>&);
    TQuaternion<T> &operator*=(T);
    TQuaternion<T> &operator/=(const TQuaternion<T>&);
    TQuaternion<T> &operator/=(T);
    TQuaternion<T> operator-() const;
    TVector3<T> operator*(const TVector3<T>&) const;

    static TQuaternion<T> FromTo(TVector3<T> from, TVector3<T> to);
    static TQuaternion<T> Reflection(const TVector3<T>& axis, const TVector3<T>& v);
    static TQuaternion<T> RotateX(T);
    static TQuaternion<T> RotateY(T);
    static TQuaternion<T> RotateZ(T);
    static TQuaternion<T> AxisAngle(const TVector3<T>&, T);
    static TQuaternion<T> LookAt(TVector3<T> dir, TVector3<T> up);
    static TQuaternion<T> Slerp(const TQuaternion<T>& a, const TQuaternion<T>& b, T t);

    TQuaternion<T> Conjugate() const;
    TQuaternion<T> Inverse() const;
    TQuaternion<T> Normal() const;
    TQuaternion<T> &Normalize();
    TQuaternion<T> &Rotate(const TVector3<T>&, T);
    T Magnitude() const;
    T SqrMagnitude() const;
    TVector3<T> GetAxis() const;
    TVector3<T> GetXAxis() const;
    TVector3<T> GetYAxis() const;
    TVector3<T> GetZAxis() const;

    static TQuaternion<T> EulerXYZ(const TVector3<T>&);
    static TQuaternion<T> EulerXZY(const TVector3<T>&);
    static TQuaternion<T> EulerYZX(const TVector3<T>&);
    static TQuaternion<T> EulerYXZ(const TVector3<T>&);
    static TQuaternion<T> EulerZXY(const TVector3<T>&);
    static TQuaternion<T> EulerZYX(const TVector3<T>&);

    TVector3<T> ToEulerXYZ() const;
    TVector3<T> ToEulerXZY() const;
    TVector3<T> ToEulerYZX() const;
    TVector3<T> ToEulerYXZ() const;
    TVector3<T> ToEulerZXY() const;
    TVector3<T> ToEulerZYX() const;
};

// 4x4矩阵主要用于三维空间的平移、旋转、大小、投影变换
template <typename T>
class TMatrix4x4 {
public:
    // OpenGL为列优先顺序
    // 此矩阵变量命名使用行优先顺序
    // 第四行对象视角变换时都为[0.0f 0.0f 0.0f 1.0f]
    // 所有运算符中仅有Vector4能参与投影变换
    T _11; T _21; T _31; T _41;
    T _12; T _22; T _32; T _42;
    T _13; T _23; T _33; T _43;
    T _14; T _24; T _34; T _44;

    static const TMatrix4x4<T> zero;
    static const TMatrix4x4<T> identity;

    TMatrix4x4<T>();
    TMatrix4x4<T>(TQuaternion<T> &&);
    TMatrix4x4<T>(TMatrix4x4<T> &&) = default;
    TMatrix4x4<T>(const TQuaternion<T> &);
    TMatrix4x4<T>(const TMatrix4x4<T> &) = default;
    TMatrix4x4<T> &operator=(TQuaternion<T> &&);
    TMatrix4x4<T> &operator=(TMatrix4x4<T> &&) = default;
    TMatrix4x4<T> &operator=(const TQuaternion<T> &);
    TMatrix4x4<T> &operator=(const TMatrix4x4<T> &) = default;
    constexpr TMatrix4x4<T>(T);
    constexpr TMatrix4x4<T>(T, T, T, T,
                            T, T, T, T,
                            T, T, T, T,
                            T, T, T, T);
    TMatrix4x4<T> operator+(const TMatrix4x4<T>&) const;
    TMatrix4x4<T> operator-(const TMatrix4x4<T>&) const;
    TMatrix4x4<T> operator*(T) const;
    TMatrix4x4<T> operator/(T) const;
    TMatrix4x4<T> &operator+=(const TMatrix4x4<T>&);
    TMatrix4x4<T> &operator-=(const TMatrix4x4<T>&);
    TMatrix4x4<T> &operator*=(T);
    TMatrix4x4<T> &operator/=(T);
    TMatrix4x4<T> operator*(const TMatrix4x4<T>&) const;
    TMatrix4x4<T> &operator*=(const TMatrix4x4<T>&);
    TMatrix4x4<T> operator-() const;
    TMatrix4x4<T> operator~() const;
    TVector3<T> operator*(const TVector2<T>&) const;
    TPoint3<T> operator*(const TPoint2<T>&) const;
    TVector3<T> operator*(const TVector3<T>&) const;
    TPoint3<T> operator*(const TPoint3<T>&) const;
    TVector4<T> operator*(const TVector4<T>&) const;

    T Determinant() const;
    TMatrix4x4<T> Inverse() const;

    TMatrix4x4<T> &AddTranslation(const TVector3<T>&);
    TMatrix4x4<T> &AddScale(const TVector3<T>&);
    TVector3<T> GetTranslation() const;
    TQuaternion<T> GetRotation() const;
    TVector3<T> GetScale() const;
};

// 3x4矩阵为4x4矩阵的简化版，第四行默认为[0.0f 0.0f 0.0f 1.0f]
// 如果transform使用此矩阵，应该能在不缺功能的情况下提升不少性能
//TODO 暂时弃用，等待实现
class Matrix3x4 final : public Object {
public:
    // 第四行默认为[0.0f 0.0f 0.0f 1.0f]
    float _11; float _12; float _13; float _14;
    float _21; float _22; float _23; float _24;
    float _31; float _32; float _33; float _34;

    static const Matrix3x4 zero;
    static const Matrix3x4 identity;

    Matrix3x4();
    Matrix3x4(Quaternion &&);
    Matrix3x4(Matrix3x4 &&) = default;
    Matrix3x4(Matrix4x4 &&);
    Matrix3x4(const Quaternion &);
    Matrix3x4(const Matrix3x4 &) = default;
    Matrix3x4(const Matrix4x4 &);
    Matrix3x4 &operator=(Quaternion &&);
    Matrix3x4 &operator=(Matrix3x4 &&) = default;
    Matrix3x4 &operator=(Matrix4x4 &&);
    Matrix3x4 &operator=(const Quaternion &);
    Matrix3x4 &operator=(const Matrix3x4 &) = default;
    Matrix3x4 &operator=(const Matrix4x4 &);
    Matrix3x4(float, float, float, float,
              float, float, float, float,
              float, float, float, float);
    Matrix3x4 operator+(Matrix3x4) const;
    Matrix3x4 operator-(Matrix3x4) const;
    Matrix3x4 operator*(float) const;
    Matrix3x4 operator/(float) const;
    Matrix3x4 &operator+=(Matrix3x4);
    Matrix3x4 &operator-=(Matrix3x4);
    Matrix3x4 &operator*=(float);
    Matrix3x4 &operator/=(float);
    Matrix3x4 operator*(Matrix3x4) const;
    Matrix3x4 &operator*=(Matrix3x4);
    Matrix3x4 operator-() const;
    Vector3 operator*(Vector2) const;
    Point3 operator*(Point2) const;
    Vector3 operator*(Vector3) const;
    Point3 operator*(Point3) const;
    Vector4 operator*(Vector4) const;

    Matrix3x4 &AddTranslation(Vector3);
    Matrix3x4 &AddScale(Vector3);
    Vector3 GetTranslation() const;
    Quaternion GetRotation() const;
    Vector3 GetScale() const;
};

// 2x3矩阵用于表示二维空间的UI变换
//TODO 暂时弃用，等待实现
class Matrix2x3 final : public Object {
public:
    // 第三行默认为[0.0f 0.0f 1.0f]
    float _11; float _12; float _13;
    float _21; float _22; float _23;

    static const Matrix2x3 zero;
    static const Matrix2x3 identity;

    Matrix2x3();
    Matrix2x3(Complex &&);
    Matrix2x3(Matrix2x3 &&) = default;
    Matrix2x3(const Complex &);
    Matrix2x3(const Matrix2x3 &) = default;
    Matrix2x3 &operator=(Complex &&);
    Matrix2x3 &operator=(Matrix2x3 &&) = default;
    Matrix2x3 &operator=(const Complex &);
    Matrix2x3 &operator=(const Matrix2x3 &) = default;
    Matrix2x3(float, float, float,
              float, float, float);
    Matrix2x3 operator+(Matrix2x3) const;
    Matrix2x3 operator-(Matrix2x3) const;
    Matrix2x3 operator*(float) const;
    Matrix2x3 operator/(float) const;
    Matrix2x3 &operator+=(Matrix2x3);
    Matrix2x3 &operator-=(Matrix2x3);
    Matrix2x3 &operator*=(float);
    Matrix2x3 &operator/=(float);
    Matrix2x3 operator*(Matrix2x3) const;
    Matrix2x3 &operator*=(Matrix2x3);
    Matrix2x3 operator-() const;
    Vector2 operator*(Vector2) const;
    Point2 operator*(Point2) const;

    Matrix2x3 &AddTranslation(Vector2);
    Matrix2x3 &AddScale(Vector2);
    Vector2 GetTranslation() const;
    Complex GetRotation() const;
    Vector2 GetScale() const;
};

// 2x4矩阵用于将三维空间坐标映射到二维UI空间坐标并加以变换
//TODO 暂时弃用，等待实现
class Matrix2x4 final : public Object {
public:
    // 第三行默认为[0.0f 0.0f 0.0f 0.0f]
    // 第四行默认为[0.0f 0.0f 0.0f 1.0f]
    float _11; float _12; float _13; float _14;
    float _21; float _22; float _23; float _24;

    static const Matrix2x4 zero;
    // 此矩阵变换X、Y坐标一致，Z坐标将不被保留
    static const Matrix2x4 identity;

    Matrix2x4();
    Matrix2x4(Quaternion &&);
    Matrix2x4(Matrix2x4 &&) = default;
    Matrix2x4(const Quaternion &);
    Matrix2x4(const Matrix2x4 &) = default;
    Matrix2x4 &operator=(Quaternion &&);
    Matrix2x4 &operator=(Matrix2x4 &&) = default;
    Matrix2x4 &operator=(const Quaternion &);
    Matrix2x4 &operator=(const Matrix2x4 &) = default;
    Matrix2x4(float, float, float, float,
              float, float, float, float);
    ~Matrix2x4();
    Matrix2x4 operator+(Matrix2x4) const;
    Matrix2x4 operator-(Matrix2x4) const;
    Matrix2x4 operator*(float) const;
    Matrix2x4 operator/(float) const;
    Matrix2x4 &operator+=(Matrix2x4);
    Matrix2x4 &operator-=(Matrix2x4);
    Matrix2x4 &operator*=(float);
    Matrix2x4 &operator/=(float);
    Matrix2x4 operator*(Matrix2x4) const;
    Matrix2x4 &operator*=(Matrix2x4);
    Matrix2x4 operator-() const;
    Vector2 operator*(Vector2) const;
    Point2 operator*(Point2) const;
    Vector2 operator*(Vector3) const;
    Point2 operator*(Point3) const;
    Vector2 operator*(Vector4) const;

    Vector2 GetTranslation() const;
    Complex GetRotation() const;
    Vector2 GetScale() const;
};

template <typename T>
class TRect {
public:
    T left;
    T right;
    T bottom;
    T top;

    static const TRect<T> zero;

    TRect<T>();
    TRect<T>(const TVector2<T>& p1, const TVector2<T>& p2);
    TRect<T>(T left, T right, T bottom, T top);

    T GetAspect() const;
    T GetWidth() const;
    T GetHeight() const;
    TVector2<T> GetSize() const;
    bool Inside(const TVector2<T>& pos) const;
    T GetXRatio(T x) const;
    T GetYRatio(T y) const;
    TVector2<T> GetRatio(T x, T y) const;
    TVector2<T> GetRatio(const TVector2<T>& pos) const;
    T GetXRatioPos(T ratio) const;
    T GetYRatioPos(T ratio) const;
    TVector2<T> GetRatioPos(T ratioX, T ratioY) const;
    TVector2<T> GetRatioPos(const TVector2<T>& ratio) const;
    T MapXPos(const TRect<T>& rect, T x) const;
    T MapYPos(const TRect<T>& rect, T y) const;
    // 把pos在rect中所在的位置比例映射到自身中位置
    TVector2<T> MapPos(const TRect<T>& rect, const TVector2<T>& pos) const;
    TVector2<T> MapPos(const TRect<T>& rect, T x, T y) const;
    TRect<T> ChildRect(const TRect<T>& ratio) const;
    TRect<T> ChildRect(T left, T right, T bottom, T top) const;
};

template <typename T>
TVector2<T>::TVector2() : x(0.0f), y(0.0f) {}
template <typename T>
TVector2<T>::TVector2(TVector3<T> &&v) : x(v.x), y(v.y) {}
template <typename T>
TVector2<T>::TVector2(TVector4<T> &&v) : x(v.x), y(v.y) {}
template <typename T>
TVector2<T>::TVector2(const TVector3<T> &v) : x(v.x), y(v.y) {}
template <typename T>
TVector2<T>::TVector2(const TVector4<T> &v) : x(v.x), y(v.y) {}

template <typename T>
TVector2<T> &TVector2<T>::operator=(TVector3<T> &&v){
    x = v.x; y = v.y;
    return *this;
}

template <typename T>
TVector2<T> &TVector2<T>::operator=(TVector4<T> &&v){
    x = v.x; y = v.y;
    return *this;
}

template <typename T>
TVector2<T> &TVector2<T>::operator=(const TVector3<T> &v){
    x = v.x; y = v.y;
    return *this;
}

template <typename T>
TVector2<T> &TVector2<T>::operator=(const TVector4<T> &v){
    x = v.x; y = v.y;
    return *this;
}

template <typename T>
constexpr TVector2<T>::TVector2(T x, T y) : x(x), y(y) {}

template <typename T>
const TVector2<T> TVector2<T>::zero     = { 0.0F, 0.0F};
template <typename T>
const TVector2<T> TVector2<T>::one      = { 1.0F, 1.0F};
template <typename T>
const TVector2<T> TVector2<T>::up       = { 0.0F, 1.0F};
template <typename T>
const TVector2<T> TVector2<T>::down     = { 0.0F,-1.0F};
template <typename T>
const TVector2<T> TVector2<T>::left     = {-1.0F, 0.0F};
template <typename T>
const TVector2<T> TVector2<T>::right    = { 1.0F, 0.0F};

template <typename T>
TVector2<T> operator*(const TVector2<T>& v, T w){
    return TVector2<T>(v.x * w, v.y * w);
}

template <typename T>
TVector2<T> operator*(T w, const TVector2<T>& v){
    return TVector2<T>(v.x * w, v.y * w);
}

template <typename T>
TVector2<T> TVector2<T>::operator*(const TVector2<T>& v) const{
    return TVector2<T>(x * v.x, y * v.y);
}

template <typename T>
TVector2<T> &TVector2<T>::operator*=(T w){
    x *= w; y *= w;
    return *this;
}

template <typename T>
TVector2<T> TVector2<T>::operator+(const TVector2<T>& v) const{
    return TVector2<T>(x + v.x, y + v.y);
}

template <typename T>
TVector2<T> &TVector2<T>::operator+=(const TVector2<T>& v){
    x += v.x; y += v.y;
    return *this;
}

template <typename T>
TVector2<T> TVector2<T>::operator-() const{
    return TVector2<T>(-x, -y);
}

template <typename T>
TVector2<T> TVector2<T>::operator-(const TVector2<T>& v) const{
    return TVector2<T>(x - v.x, y - v.y);
}

template <typename T>
TVector2<T> &TVector2<T>::operator-=(const TVector2<T>& v){
    x -= v.x; y -= v.y;
    return *this;
}

template <typename T>
TVector2<T> TVector2<T>::operator/(T w) const{
    w = 1.0F / w;
    return TVector2<T>(x * w, y * w);
}

// 我总觉得类型萃取什么的可能影响性能
// 而且编译已经很慢了>_<
template <>
TVector2<int> TVector2<int>::operator/(int w) const;

template <typename T>
TVector2<T> TVector2<T>::operator/(const TVector2<T>& v) const{
    return TVector2<T>(x / v.x, y / v.y);
}

template <typename T>
TVector2<T> &TVector2<T>::operator/=(T w){
    w = 1.0F / w;
    x *= w; y *= w;
    return *this;
}

template <>
TVector2<int> &TVector2<int>::operator/=(int w);

template <typename T>
T TVector2<T>::Cross(const TVector2<T>& v1, const TVector2<T>& v2){
    return v1.x * v2.y - v1.y * v2.x;
}

template <typename T>
T TVector2<T>::Dot(const TVector2<T>& v1, const TVector2<T>& v2){
    return v1.x * v2.x + v1.y * v2.y;
}

template <typename T>
T TVector2<T>::Cosine(const TVector2<T>& v1, const TVector2<T>& v2){
    return Dot(v1, v2) / Sqrt(v1.SqrMagnitude() * v2.SqrMagnitude());
}

template <typename T>
T TVector2<T>::Sine(const TVector2<T>& v1, const TVector2<T>& v2){
    T val = Cosine(v1, v2);
    return Sqrt(1.0F - val * val);
}

template <typename T>
T TVector2<T>::Tangent(const TVector2<T>& v1, const TVector2<T>& v2){
    T val = Cosine(v1, v2);
    return Sqrt(1.0F - val * val) / val;
}

template <typename T>
T TVector2<T>::Magnitude() const{
    return Sqrt(x * x + y * y);
}

template <typename T>
T TVector2<T>::SqrMagnitude() const{
    return x * x + y * y;
}

template <typename T>
TVector2<T> TVector2<T>::Normal() const{
    return *this / this->Magnitude();
}

template <typename T>
TVector2<T> &TVector2<T>::Normalize(){
    return (*this /= this->Magnitude());
}

template <typename T>
TVector2<T> TVector2<T>::Reflect(TVector2<T> n) const{
    n *= (Dot(*this, n) / n.SqrMagnitude());
    return *this - n * 2.0F;
}

template <typename T>
TVector2<T> TVector2<T>::Refract(TVector2<T> n, T rate) const{
    n *= (Dot(*this, n) / n.SqrMagnitude());
    T val1 = Sine(*this, n);
    T val2 = val1 / rate;
    return n + (*this - n) * val2 / Sqrt(1.0F - val2 * val2) / val1 * Sqrt(1.0F - val1 * val1);
}

template <typename T>
TVector2<T> TVector2<T>::Rotate(T angle) const{
    T vsin, vcos;
    SinCos(ToRadian(angle), &vsin, &vcos);
    return TVector2<T>(x * vcos - y * vsin, x * vsin + y * vcos);
}

template <typename T>
TPoint2<T>::TPoint2() : TVector2<T>(0.0f, 0.0f) {}
template <typename T>
TPoint2<T>::TPoint2(TVector2<T> &&v) : TVector2<T>(v) {}
template <typename T>
TPoint2<T>::TPoint2(const TVector2<T> &v) : TVector2<T>(v) {}

template <typename T>
TPoint2<T> &TPoint2<T>::operator=(TVector2<T> &&v){
    TVector2<T>::x = v.x; TVector2<T>::y = v.y;
    return *this;
}

template <typename T>
TPoint2<T> &TPoint2<T>::operator=(const TVector2<T> &v){
    TVector2<T>::x = v.x; TVector2<T>::y = v.y;
    return *this;
}

template <typename T>
constexpr TPoint2<T>::TPoint2(T x, T y) : TVector2<T>(x, y) {}

template <typename T>
const TPoint2<T> TPoint2<T>::zero       = { 0.0F, 0.0F};

template <typename T>
TVector3<T>::TVector3() : x(0.0f), y(0.0f), z(0.0f) {}
template <typename T>
TVector3<T>::TVector3(TVector2<T> &&v) : x(v.x), y(v.y), z(0.0f) {}
template <typename T>
TVector3<T>::TVector3(TVector4<T> &&v) : x(v.x), y(v.y), z(v.z) {}
template <typename T>
TVector3<T>::TVector3(const TVector2<T> &v) : x(v.x), y(v.y), z(0.0f) {}
template <typename T>
TVector3<T>::TVector3(const TVector4<T> &v) : x(v.x), y(v.y), z(v.z) {}

template <typename T>
TVector3<T> &TVector3<T>::operator=(TVector2<T> &&v){
    this->x = v.x; this->y = v.y; this->z = 0.0f;
    return *this;
}

template <typename T>
TVector3<T> &TVector3<T>::operator=(TVector4<T> &&v){
    x = v.x; y = v.y; z = v.z;
    return *this;
}

template <typename T>
TVector3<T> &TVector3<T>::operator=(const TVector2<T> &v){
    this->x = v.x; this->y = v.y; this->z = 0.0f;
    return *this;
}

template <typename T>
TVector3<T> &TVector3<T>::operator=(const TVector4<T> &v){
    x = v.x; y = v.y; z = v.z;
    return *this;
}

template <typename T>
constexpr TVector3<T>::TVector3(T x, T y) : x(x), y(y), z(0.0f) {}
template <typename T>
constexpr TVector3<T>::TVector3(T x, T y, T z) : x(x), y(y), z(z) {}

template <typename T>
const TVector3<T> TVector3<T>::zero     = { 0.0F, 0.0F, 0.0F};
template <typename T>
const TVector3<T> TVector3<T>::one      = { 1.0F, 1.0F, 1.0F};
template <typename T>
const TVector3<T> TVector3<T>::up       = { 0.0F, 0.0F, 1.0F};
template <typename T>
const TVector3<T> TVector3<T>::down     = { 0.0F, 0.0F,-1.0F};
template <typename T>
const TVector3<T> TVector3<T>::left     = {-1.0F, 0.0F, 0.0F};
template <typename T>
const TVector3<T> TVector3<T>::right    = { 1.0F, 0.0F, 0.0F};
template <typename T>
const TVector3<T> TVector3<T>::forward  = { 0.0F, 1.0F, 0.0F};
template <typename T>
const TVector3<T> TVector3<T>::back     = { 0.0F,-1.0F, 0.0F};

template <typename T>
TVector3<T> operator*(const TVector3<T>& v, T w){
    return TVector3<T>(v.x * w, v.y * w, v.z * w);
}

template <typename T>
TVector3<T> operator*(T w, const TVector3<T>& v){
    return TVector3<T>(v.x * w, v.y * w, v.z * w);
}

template <typename T>
TVector3<T> TVector3<T>::operator*(const TVector3<T>& v) const{
    return TVector3<T>(x * v.x, y * v.y, z * v.z);
}

template <typename T>
TVector3<T> &TVector3<T>::operator*=(T w){
    x *= w; y *= w; z *= w;
    return *this;
}

template <typename T>
TVector3<T> TVector3<T>::operator+(const TVector3<T>& v) const{
    return TVector3<T>(x + v.x, y + v.y, z + v.z);
}

template <typename T>
TVector3<T> &TVector3<T>::operator+=(const TVector3<T>& v){
    x += v.x; y += v.y; z += v.z;
    return *this;
}

template <typename T>
TVector3<T> TVector3<T>::operator-() const{
    return TVector3<T>(-x, -y, -z);
}

template <typename T>
TVector3<T> TVector3<T>::operator-(const TVector3<T>& v) const{
    return TVector3<T>(x - v.x, y - v.y, z - v.z);
}

template <typename T>
TVector3<T> &TVector3<T>::operator-=(const TVector3<T>& v){
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
}

template <typename T>
TVector3<T> TVector3<T>::operator/(T w) const{
    w = 1.0F / w;
    return TVector3<T>(x * w, y * w, z * w);
}

template <>
TVector3<int> TVector3<int>::operator/(int w) const;

template <typename T>
TVector3<T> TVector3<T>::operator/(const TVector3<T>& v) const{
    return TVector3<T>(x / v.x, y / v.y, z / v.z);
}

template <typename T>
TVector3<T> &TVector3<T>::operator/=(T w){
    w = 1.0F / w;
    x *= w; y *= w; z *= w;
    return *this;
}

template <>
TVector3<int> &TVector3<int>::operator/=(int w);

template <typename T>
TVector3<T> &TVector3<T>::operator*=(const TQuaternion<T>& q){
    return (*this = q * *this);
}

template <typename T>
TVector3<T> &TVector3<T>::operator/=(const TQuaternion<T>& q){
    return (*this = q.Inverse() * *this);
}

template <typename T>
TVector3<T> TVector3<T>::Cross(const TVector3<T>& v1, const TVector3<T>& v2){
    return TVector3<T>( v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x);
}

template <typename T>
T TVector3<T>::Dot(const TVector3<T>& v1, const TVector3<T>& v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
T TVector3<T>::Cosine(const TVector3<T>& v1, const TVector3<T>& v2){
    return Dot(v1, v2) / Sqrt(v1.SqrMagnitude() * v2.SqrMagnitude());
}

template <typename T>
T TVector3<T>::Sine(const TVector3<T>& v1, const TVector3<T>& v2){
    T val = Cosine(v1, v2);
    return Sqrt(1.0F - val * val);
}

template <typename T>
T TVector3<T>::Tangent(const TVector3<T>& v1, const TVector3<T>& v2){
    T val = Cosine(v1, v2);
    return Sqrt(1.0F - val * val) / val;
}

template <typename T>
T TVector3<T>::Determinant(const TVector3<T>& v1, const TVector3<T>& v2, const TVector3<T>& v3){
    return v1.x * (v2.y * v3.z - v2.z * v3.y) +
           v1.y * (v2.z * v3.x - v2.x * v3.z) +
           v1.z * (v2.x * v3.y - v2.y * v3.x);
}

template <typename T>
TVector3<T> TVector3<T>::Compose(const TVector3<T>& coord, const TVector3<T>& x, const TVector3<T>& y, const TVector3<T>& z){
    return x * coord.x + y * coord.y + z * coord.z;
}

template <typename T>
TVector3<T> TVector3<T>::Decompose(const TVector3<T>& vec, const TVector3<T>& x, const TVector3<T>& y, const TVector3<T>& z){
    T detinv = 1.0f / Determinant(x, y, z);
    return TVector3<T>(Determinant(vec, y, z) * detinv,
                       Determinant(x, vec, z) * detinv,
                       Determinant(x, y, vec) * detinv);
}

template <typename T>
TVector3<T> TVector3<T>::Decompose(const TVector3<T>& vec, const TVector3<T>& x, const TVector3<T>& y, const TVector3<T>& z, T det){
    T detinv = 1.0f / det;
    return TVector3<T>(Determinant(vec, y, z) * detinv,
                       Determinant(x, vec, z) * detinv,
                       Determinant(x, y, vec) * detinv);
}

template <typename T>
T TVector3<T>::Magnitude() const{
    return Sqrt(x * x + y * y + z * z);
}

template <typename T>
T TVector3<T>::SqrMagnitude() const{
    return x * x + y * y + z * z;
}

template <typename T>
TVector3<T> TVector3<T>::Normal() const{
    return *this / this->Magnitude();
}

template <typename T>
TVector3<T> &TVector3<T>::Normalize(){
    return (*this /= this->Magnitude());
}

template <typename T>
TVector3<T> TVector3<T>::Reflect(TVector3<T> n) const{
    n *= (Dot(*this, n) / n.SqrMagnitude());
    return *this - n * 2.0F;
}

template <typename T>
TVector3<T> TVector3<T>::Refract(TVector3<T> n, T rate) const{
    n *= (Dot(*this, n) / n.SqrMagnitude());
    T val1 = Sine(*this, n);
    T val2 = val1 / rate;
    return n + (*this - n) * val2 / Sqrt(1.0F - val2 * val2) / val1 * Sqrt(1.0F - val1 * val1);
}

template <typename T>
TVector3<T> TVector3<T>::RotateX(T angle) const{
    T vsin, vcos;
    SinCos(ToRadian(angle), &vsin, &vcos);
    return TVector3<T>(x, z * vsin + y * vcos, z * vcos - y * vsin);
}

template <typename T>
TVector3<T> TVector3<T>::RotateY(T angle) const{
    T vsin, vcos;
    SinCos(ToRadian(angle), &vsin, &vcos);
    return TVector3<T>(z * vsin + x * vcos, y, z * vcos - x * vsin);
}

template <typename T>
TVector3<T> TVector3<T>::RotateZ(T angle) const{
    T vsin, vcos;
    SinCos(ToRadian(angle), &vsin, &vcos);
    return TVector3<T>(x * vcos - y * vsin, x * vsin + y * vcos, z);
}

template <typename T>
TVector2<T> TVector3<T>::XY() const{
    return TVector2<T>(x, y);
}

template <typename T>
TVector2<T> TVector3<T>::YZ() const{
    return TVector2<T>(y, z);
}

template <typename T>
TVector2<T> TVector3<T>::ZX() const{
    return TVector2<T>(z, x);
}

template <typename T>
TVector2<T> TVector3<T>::YX() const{
    return TVector2<T>(y, x);
}

template <typename T>
TVector2<T> TVector3<T>::ZY() const{
    return TVector2<T>(z, y);
}

template <typename T>
TVector2<T> TVector3<T>::XZ() const{
    return TVector2<T>(x, z);
}

template <typename T>
TPoint3<T>::TPoint3() : TVector3<T>(0.0f, 0.0f, 0.0f) {}
template <typename T>
TPoint3<T>::TPoint3(TVector3<T> &&v) : TVector3<T>(v) {}
template <typename T>
TPoint3<T>::TPoint3(const TVector3<T> &v) : TVector3<T>(v) {}

template <typename T>
TPoint3<T> &TPoint3<T>::operator=(TVector3<T> &&v){
    TVector3<T>::x = v.x; TVector3<T>::y = v.y; TVector3<T>::z = v.z;
    return *this;
}

template <typename T>
TPoint3<T> &TPoint3<T>::operator=(const TVector3<T> &v){
    TVector3<T>::x = v.x; TVector3<T>::y = v.y; TVector3<T>::z = v.z;
    return *this;
}

template <typename T>
constexpr TPoint3<T>::TPoint3(T x, T y) : TVector3<T>(x, y, 0.0f) {}
template <typename T>
constexpr TPoint3<T>::TPoint3(T x, T y, T z) : TVector3<T>(x, y, z) {}

template <typename T>
const TPoint3<T> TPoint3<T>::zero       = { 0.0F, 0.0F, 0.0F};

template <typename T>
TVector4<T>::TVector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
template <typename T>
TVector4<T>::TVector4(TVector2<T> &&v) : x(v.x), y(v.y), z(0.0f), w(0.0f) {}
template <typename T>
TVector4<T>::TVector4(TPoint2<T> &&v) : x(v.x), y(v.y), z(0.0f), w(1.0f) {}
template <typename T>
TVector4<T>::TVector4(TVector3<T> &&v) : x(v.x), y(v.y), z(v.z), w(0.0f) {}
template <typename T>
TVector4<T>::TVector4(TPoint3<T> &&v) : x(v.x), y(v.y), z(v.z), w(1.0f) {}
template <typename T>
TVector4<T>::TVector4(const TVector2<T> &v) : x(v.x), y(v.y), z(0.0f), w(0.0f) {}
template <typename T>
TVector4<T>::TVector4(const TPoint2<T> &v) : x(v.x), y(v.y), z(0.0f), w(1.0f) {}
template <typename T>
TVector4<T>::TVector4(const TVector3<T> &v) : x(v.x), y(v.y), z(v.z), w(0.0f) {}
template <typename T>
TVector4<T>::TVector4(const TPoint3<T> &v) : x(v.x), y(v.y), z(v.z), w(1.0f) {}

template <typename T>
TVector4<T> &TVector4<T>::operator=(TVector2<T> &&v){
    this->x = v.x; this->y = v.y; this->z = 0.0f; this->w = 0.0f;
    return *this;
}

template <typename T>
TVector4<T> &TVector4<T>::operator=(TPoint2<T> &&v){
    this->x = v.x; this->y = v.y; this->z = 0.0f; this->w = 1.0f;
    return *this;
}

template <typename T>
TVector4<T> &TVector4<T>::operator=(TVector3<T> &&v){
    this->x = v.x; this->y = v.y; this->z = v.z; this->w = 0.0f;
    return *this;
}

template <typename T>
TVector4<T> &TVector4<T>::operator=(TPoint3<T> &&v){
    this->x = v.x; this->y = v.y; this->z = v.z; this->w = 1.0f;
    return *this;
}

template <typename T>
TVector4<T> &TVector4<T>::operator=(const TVector2<T> &v){
    this->x = v.x; this->y = v.y; this->z = 0.0f; this->w = 0.0f;
    return *this;
}

template <typename T>
TVector4<T> &TVector4<T>::operator=(const TPoint2<T> &v){
    this->x = v.x; this->y = v.y; this->z = 0.0f; this->w = 1.0f;
    return *this;
}

template <typename T>
TVector4<T> &TVector4<T>::operator=(const TVector3<T> &v){
    this->x = v.x; this->y = v.y; this->z = v.z; this->w = 0.0f;
    return *this;
}

template <typename T>
TVector4<T> &TVector4<T>::operator=(const TPoint3<T> &v){
    this->x = v.x; this->y = v.y; this->z = v.z; this->w = 1.0f;
    return *this;
}

template <typename T>
constexpr TVector4<T>::TVector4(T x, T y) : x(x), y(y), z(0.0f), w(1.0f) {}
template <typename T>
constexpr TVector4<T>::TVector4(T x, T y, T z) : x(x), y(y), z(z), w(1.0f) {}
template <typename T>
constexpr TVector4<T>::TVector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
template <typename T>
TVector4<T>::TVector4(const TVector3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}

template <typename T>
const TVector4<T> TVector4<T>::zero     = { 0.0F, 0.0F, 0.0F, 0.0F};
template <typename T>
const TVector4<T> TVector4<T>::one      = { 1.0F, 1.0F, 1.0F, 0.0F};
template <typename T>
const TVector4<T> TVector4<T>::up       = { 0.0F, 0.0F, 1.0F, 0.0F};
template <typename T>
const TVector4<T> TVector4<T>::down     = { 0.0F, 0.0F,-1.0F, 0.0F};
template <typename T>
const TVector4<T> TVector4<T>::left     = {-1.0F, 0.0F, 0.0F, 0.0F};
template <typename T>
const TVector4<T> TVector4<T>::right    = { 1.0F, 0.0F, 0.0F, 0.0F};
template <typename T>
const TVector4<T> TVector4<T>::forward  = { 0.0F, 1.0F, 0.0F, 0.0F};
template <typename T>
const TVector4<T> TVector4<T>::back     = { 0.0F,-1.0F, 0.0F, 0.0F};

template <typename T>
TVector4<T> TVector4<T>::operator+(const TVector3<T>& v) const{
    return TVector4<T>(x + v.x, y + v.y, z + v.z, w);
}

template <typename T>
TVector4<T> TVector4<T>::operator+(const TVector4<T>& v) const{
    return TVector4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
}

template <typename T>
TVector4<T> TVector4<T>::operator-(const TVector3<T>& v) const{
    return TVector4<T>(x - v.x, y - v.y, z - v.z, w);
}

template <typename T>
TVector4<T> TVector4<T>::operator-(const TVector4<T>& v) const{
    return TVector4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
}

template <typename T>
TVector4<T> TVector4<T>::operator*(T w) const{
    return TVector4<T>(x * w, y * w, z * w, this->w);
}

template <typename T>
TVector4<T> TVector4<T>::operator/(T w) const{
    w = 1.0F / w;
    return TVector4<T>(x * w, y * w, z * w, this->w);
}

template <>
TVector4<int> TVector4<int>::operator/(int w) const;

template <typename T>
TVector4<T> &TVector4<T>::operator+=(const TVector3<T>& v){
    x += v.x; y += v.y; z += v.z;
    return *this;
}

template <typename T>
TVector4<T> &TVector4<T>::operator-=(const TVector3<T>& v){
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
}

template <typename T>
TVector4<T> &TVector4<T>::operator*=(T w){
    x *= w; y *= w; z *= w;
    return *this;
}

template <typename T>
TVector4<T> &TVector4<T>::operator/=(T w){
    w = 1.0F / w;
    x *= w; y *= w; z *= w;
    return *this;
}

template <>
TVector4<int> &TVector4<int>::operator/=(int w);

template <typename T>
TVector4<T> TVector4<T>::operator-() const{
    return TVector4<T>(-x, -y, -z, w);
}

template <typename T>
TVector4<T> &TVector4<T>::operator*=(const TQuaternion<T>& q){
    return (*this = q * *this);
}

template <typename T>
TVector4<T> &TVector4<T>::operator/=(const TQuaternion<T>& q){
    return (*this = q.Inverse() * *this);
}

template <typename T>
T TVector4<T>::Dot(const TVector4<T>& v1, const TVector4<T>& v2){
    return TVector3<T>::Dot(v1, v2);
}

template <typename T>
TVector4<T> TVector4<T>::Cross(const TVector4<T>& v1, const TVector4<T>& v2){
    return TVector3<T>::Cross(v1, v2);
}

template <typename T>
T TVector4<T>::Cosine(const TVector4<T>& v1, const TVector4<T>& v2){
    return TVector3<T>::Cosine(v1, v2);
}

template <typename T>
T TVector4<T>::Sine(const TVector4<T>& v1, const TVector4<T>& v2){
    return TVector3<T>::Sine(v1, v2);
}

template <typename T>
T TVector4<T>::Tangent(const TVector4<T>& v1, const TVector4<T>& v2){
    return TVector3<T>::Tangent(v1, v2);
}

template <typename T>
TVector3<T> TVector4<T>::Normal() const{
    return TVector3<T>(x, y, z).Normal();
}

template <typename T>
TVector4<T> &TVector4<T>::Normalize(){
    return (*this = TVector3<T>(x, y, z).Normal());
}

template <typename T>
T TVector4<T>::Magnitude() const{
    return Sqrt(x * x + y * y + z * z);
}

template <typename T>
T TVector4<T>::SqrMagnitude() const{
    return x * x + y * y + z * z;
}

template <typename T>
TVector4<T> TVector4<T>::Reflect(TVector3<T> n) const{
    n *= (Dot(*this, n) / n.SqrMagnitude());
    return *this - n * 2.0F;
}

template <typename T>
TVector4<T> TVector4<T>::Refract(TVector3<T> n, T rate) const{
    return TVector3<T>(*this).Refract(n, rate);
}

template <typename T>
TVector4<T> TVector4<T>::RotateX(T angle) const{
    return TVector3<T>(*this).RotateX(angle);
}

template <typename T>
TVector4<T> TVector4<T>::RotateY(T angle) const{
    return TVector3<T>(*this).RotateY(angle);
}

template <typename T>
TVector4<T> TVector4<T>::RotateZ(T angle) const{
    return TVector3<T>(*this).RotateZ(angle);
}

template <typename T>
constexpr TComplex<T>::TComplex() : real(0.0f), imag(0.0f) {}
template <typename T>
TComplex<T>::TComplex(T &&real) : real(real), imag(0.0f) {}
template <typename T>
TComplex<T>::TComplex(TVector2<T> &&v) : real(v.x), imag(v.y) {}
template <typename T>
TComplex<T>::TComplex(const T &real) : real(real), imag(0.0f) {}
template <typename T>
TComplex<T>::TComplex(const TVector2<T> &v) : real(v.x), imag(v.y) {}

template <typename T>
TComplex<T> &TComplex<T>::operator=(T &&real){
    this->real = real; this->imag = 0.0f;
    return *this;
}

template <typename T>
TComplex<T> &TComplex<T>::operator=(TVector2<T> &&v){
    this->real = v.x; this->imag = v.y;
    return *this;
}

template <typename T>
TComplex<T> &TComplex<T>::operator=(const T &real){
    this->real = real; this->imag = 0.0f;
    return *this;
}

template <typename T>
TComplex<T> &TComplex<T>::operator=(const TVector2<T> &v){
    this->real = v.x; this->imag = v.y;
    return *this;
}

template <typename T>
constexpr TComplex<T>::TComplex(T real, T imag) : real(real), imag(imag) {}

template <typename T>
const TComplex<T> TComplex<T>::one          = { 1.0F, 0.0F};
template <typename T>
const TComplex<T> TComplex<T>::i            = { 0.0F, 1.0F};

template <typename T>
TComplex<T> TComplex<T>::operator+(const TComplex<T>& c) const{
    return TComplex<T>(real + c.real, imag + c.imag);
}

template <typename T>
TComplex<T> operator+(const TComplex<T>& c, T x){
    return TComplex<T>(c.real + x, c.imag);
}

template <typename T>
TComplex<T> operator+(T x, const TComplex<T>& c){
    return TComplex<T>(c.real + x, c.imag);
}

template <typename T>
TComplex<T> TComplex<T>::operator-(const TComplex<T>& c) const{
    return TComplex<T>(real - c.real, imag - c.imag);
}

template <typename T>
TComplex<T> operator-(const TComplex<T>& c, T x){
    return TComplex<T>(c.real - x, c.imag);
}

template <typename T>
TComplex<T> operator-(T x, const TComplex<T>& c){
    return TComplex<T>(x - c.real, -c.imag);
}

template <typename T>
TComplex<T> TComplex<T>::operator*(const TComplex<T>& c) const{
    return TComplex<T>(
        real * c.real - imag * c.imag,
        real * c.imag + imag * c.real
    );
}

template <typename T>
TComplex<T> operator*(const TComplex<T>& c, T x){
    return TComplex<T>(c.real * x, c.imag * x);
}

template <typename T>
TComplex<T> operator*(T x, const TComplex<T>& c){
    return TComplex<T>(c.real * x, c.imag * x);
}

template <typename T>
TComplex<T> TComplex<T>::operator/(const TComplex<T>& c) const{
    return *this * c.Inverse();
}

template <typename T>
TComplex<T> operator/(const TComplex<T>& c, T x){
    T inv = 1.0F / x;
    return TComplex<T>(c.real * inv, c.imag * inv);
}

template <>
TComplex<int> operator/(const TComplex<int>& c, int x);

template <typename T>
TComplex<T> operator/(T x, const TComplex<T>& c){
    return x * c.Inverse();
}

template <typename T>
TComplex<T> &TComplex<T>::operator+=(const TComplex<T>& c){
    real += c.real; imag += c.imag;
    return *this;
}

template <typename T>
TComplex<T> &TComplex<T>::operator+=(T x){
    real += x;
    return *this;
}

template <typename T>
TComplex<T> &TComplex<T>::operator-=(const TComplex<T>& c){
    real -= c.real; imag -= c.imag;
    return *this;
}

template <typename T>
TComplex<T> &TComplex<T>::operator-=(T x){
    real -= x;
    return *this;
}

template <typename T>
TComplex<T> &TComplex<T>::operator*=(const TComplex<T>& c){
    return (*this = *this * c);
}

template <typename T>
TComplex<T> &TComplex<T>::operator*=(T x){
    real *= x; imag *= x;
    return *this;
}

template <typename T>
TComplex<T> &TComplex<T>::operator/=(const TComplex<T>& c){
    return (*this = *this / c);
}

template <typename T>
TComplex<T> &TComplex<T>::operator/=(T x){
    T inv = 1.0F / x;
    real *= inv; imag *= inv;
    return *this;
}

template <>
TComplex<int> &TComplex<int>::operator/=(int x);

template <typename T>
TComplex<T> TComplex<T>::operator-() const{
    return TComplex<T>(-real, -imag);
}

template <typename T>
TVector2<T> TComplex<T>::operator*(const TVector2<T>& v) const{
    return TVector2<T>(real * v.x - imag * v.y, real * v.y + imag * v.x);
}

template <typename T>
TComplex<T> TComplex<T>::FromTo(TVector2<T> from, TVector2<T> to){
    return (TComplex<T>(to) / TComplex<T>(from)).Normal();
}

template <typename T>
TComplex<T> TComplex<T>::Reflection(TVector2<T> axis, TVector2<T> v){
    return Square(TComplex<T>(axis) / TComplex<T>(i)).Normal();
}

template <typename T>
TComplex<T> TComplex<T>::Rotation(T angle){
    T vsin, vcos;
    SinCos(ToRadian(angle), &vsin, &vcos);
    return TComplex<T>(vcos, vsin);
}

template <typename T>
TComplex<T> TComplex<T>::Conjugate() const{
    return TComplex<T>(real, -imag);
}

template <typename T>
TComplex<T> TComplex<T>::Inverse() const{
    return TComplex<T>(real, -imag) / SqrMagnitude();
}

template <typename T>
TComplex<T> TComplex<T>::Normal() const{
    return *this / Magnitude();
}

template <typename T>
TComplex<T> &TComplex<T>::Normalize(){
    return (*this /= Magnitude());
}

template <typename T>
TComplex<T> &TComplex<T>::Rotate(T angle){
    return (*this *= Rotation(angle));
}

template <typename T>
T TComplex<T>::Magnitude() const{
    return Sqrt(real * real + imag * imag);
}

template <typename T>
T TComplex<T>::SqrMagnitude() const{
    return real * real + imag * imag;
}

template <typename T>
constexpr TQuaternion<T>::TQuaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
template <typename T>
constexpr TQuaternion<T>::TQuaternion(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
template <typename T>
TQuaternion<T>::TQuaternion(const TVector3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}

template <typename T>
const TQuaternion<T> TQuaternion<T>::one     = { 0.0F, 0.0F, 0.0F, 1.0F};

template <typename T>
TQuaternion<T> TQuaternion<T>::operator*(const TQuaternion<T>& q) const{
    return TQuaternion<T>(w * q.x + x * q.w + y * q.z - z * q.y,
                          w * q.y - x * q.z + y * q.w + z * q.x,
                          w * q.z + x * q.y - y * q.x + z * q.w,
                          w * q.w - x * q.x - y * q.y - z * q.z);
    //(w,z,-y,-x) * (-x,-y,-z,w) = (x2+w2-z2-y2,xy+wz-yw+yx,xz-wy+zx-yw,0)
}

template <typename T>
TQuaternion<T> TQuaternion<T>::operator*(T s) const{
    return TQuaternion<T>(x * s, y * s, z * s, w * s);
}

template <typename T>
TQuaternion<T> TQuaternion<T>::operator/(const TQuaternion<T>& q) const{
    return *this * q.Inverse();
}

template <typename T>
TQuaternion<T> TQuaternion<T>::operator/(T s) const{
    s = 1.0F / s;
    return TQuaternion<T>(x * s, y * s, z * s, w * s);
}

template <>
TQuaternion<int> TQuaternion<int>::operator/(int s) const;

template <typename T>
TQuaternion<T> &TQuaternion<T>::operator*=(const TQuaternion<T>& q){
    return (*this = *this * q);
}

template <typename T>
TQuaternion<T> &TQuaternion<T>::operator*=(T s){
    x *= s; y *= s; z *= s; w *= s;
    return *this;
}

template <typename T>
TQuaternion<T> &TQuaternion<T>::operator/=(const TQuaternion<T>& q){
    return (*this *= *this * q.Inverse());
}

template <typename T>
TQuaternion<T> &TQuaternion<T>::operator/=(T s){
    s = 1.0F / s;
    x *= s; y *= s; z *= s; w *= s;
    return *this;
}

template <>
TQuaternion<int> &TQuaternion<int>::operator/=(int s);

template <typename T>
TQuaternion<T> TQuaternion<T>::operator-() const{
    return TQuaternion<T>(-x, -y, -z, w);
}

template <typename T>
TVector3<T> TQuaternion<T>::operator*(const TVector3<T>& v) const{
    return (*this * TQuaternion<T>(v, 0.0F) * this->Inverse()).GetAxis();
}

template <typename T>
TQuaternion<T> TQuaternion<T>::FromTo(TVector3<T> from, TVector3<T> to){
    from.Normalize();
    to.Normalize();
    if ((from + to).SqrMagnitude() == 0.0f)
        return TQuaternion<T>(TVector3<T>::Cross(from, ((from - TVector3<T>::forward).SqrMagnitude() == 0.0f ? TVector3<T>::right : TVector3<T>::forward)).Normal(), 0.0f);
    return -(TQuaternion<T>((from + to).Normal(), 0.0F) / TQuaternion<T>(from, 0.0F));
}

template <typename T>
TQuaternion<T> TQuaternion<T>::Reflection(const TVector3<T>& axis, const TVector3<T>& v){
    return (TQuaternion<T>(axis, 0.0F) / TQuaternion<T>(v, 0.0F)).Normal();
}

template <typename T>
TQuaternion<T> TQuaternion<T>::RotateX(T angle){
    T vsin, vcos;
    SinCos(ToRadianHalf(angle), &vsin, &vcos);
    return TQuaternion<T>(vsin, 0.0f, 0.0f, vcos);
}

template <typename T>
TQuaternion<T> TQuaternion<T>::RotateY(T angle){
    T vsin, vcos;
    SinCos(ToRadianHalf(angle), &vsin, &vcos);
    return TQuaternion<T>(0.0f, vsin, 0.0f, vcos);
}

template <typename T>
TQuaternion<T> TQuaternion<T>::RotateZ(T angle){
    T vsin, vcos;
    SinCos(ToRadianHalf(angle), &vsin, &vcos);
    return TQuaternion<T>(0.0f, 0.0f, vsin, vcos);
}

template <typename T>
TQuaternion<T> TQuaternion<T>::AxisAngle(const TVector3<T>& axis, T angle){
    T vsin, vcos;
    SinCos(ToRadianHalf(angle), &vsin, &vcos);
    return TQuaternion<T>(axis.Normal() * vsin, vcos);
}

template <typename T>
TQuaternion<T> TQuaternion<T>::LookAt(TVector3<T> dir, TVector3<T> up){
    dir.Normalize();
    TQuaternion<T> res = TQuaternion<T>::FromTo(TVector3<T>::forward, dir);
    up -= dir * TVector3<T>::Dot(dir, up);
    return TQuaternion<T>::FromTo(res * TVector3<T>::up, up) * res;
}

template <typename T>
TQuaternion<T> TQuaternion<T>::Slerp(const TQuaternion<T>& a, const TQuaternion<T>& b, T t){
    TQuaternion<T> dif = b / a;
    T angle = Acos(dif.w);
    return TQuaternion<T>::AxisAngle(dif.GetAxis(), angle * t) * a;
}

template <typename T>
TQuaternion<T> TQuaternion<T>::Conjugate() const{
    return TQuaternion<T>(-x, -y, -z, w);
}

template <typename T>
TQuaternion<T> TQuaternion<T>::Inverse() const{
    return TQuaternion<T>(-x, -y, -z, w) / this->SqrMagnitude();
}

template <typename T>
TQuaternion<T> TQuaternion<T>::Normal() const{
    return *this / this->Magnitude();
}

template <typename T>
TQuaternion<T> &TQuaternion<T>::Normalize(){
    return (*this /= this->Magnitude());
}

template <typename T>
TQuaternion<T> &TQuaternion<T>::Rotate(const TVector3<T>& axis, T angle){
    return (*this *= AxisAngle(-*this * axis, angle) * *this);
}

template <typename T>
T TQuaternion<T>::Magnitude() const{
    return Sqrt(x * x + y * y + z * z + w * w);
}

template <typename T>
T TQuaternion<T>::SqrMagnitude() const{
    return x * x + y * y + z * z + w * w;
}

template <typename T>
TVector3<T> TQuaternion<T>::GetAxis() const{
    return TVector3<T>(x, y, z);
}

template <typename T>
TVector3<T> TQuaternion<T>::GetXAxis() const{
    return TVector3<T>(
        1.0f - 2.0f * (y * y + z * z),
        2.0f * (x * y + z * w),
        2.0f * (x * z + y * w)
    );
}

template <typename T>
TVector3<T> TQuaternion<T>::GetYAxis() const{
    return TVector3<T>(
        2.0f * (x * y - z * w),
        1.0f - 2.0f * (z * z + x * x),
        2.0f * (y *z + x * w)
    );
}

template <typename T>
TVector3<T> TQuaternion<T>::GetZAxis() const{
    return TVector3<T>(
        2.0f * (x * z - y * w),
        2.0f * (y * z - x * w),
        1.0f - 2.0f * (x * x + y * y)
    );
}

// 欧拉角 => [w, x, y, z]
// x => [cos(a/2), sin(a/2), 0, 0]
// y => [cos(a/2), 0, sin(a/2), 0]
// z => [cos(a/2), 0, 0, sin(a/2)]
template <typename T>
TQuaternion<T> TQuaternion<T>::EulerXYZ(const TVector3<T>& angle){
    T sx, cx, sy, cy, sz, cz;

    SinCos(ToRadianHalf(angle.x), &sx, &cx);
    SinCos(ToRadianHalf(angle.y), &sy, &cy);
    SinCos(ToRadianHalf(angle.z), &sz, &cz);
    
    // XYZ CCW
    return TQuaternion<T>(
        cz * sx * cy - sz * cx * sy,
        cz * cx * sy + sz * sx * cy,
        sz * cx * cy - cz * sx * sy,
        cz * cx * cy + sz * sx * sy
    );
}

template <typename T>
TQuaternion<T> TQuaternion<T>::EulerXZY(const TVector3<T>& angle){
    T sx, cx, sy, cy, sz, cz;

    SinCos(ToRadianHalf(angle.x), &sx, &cx);
    SinCos(ToRadianHalf(angle.y), &sy, &cy);
    SinCos(ToRadianHalf(angle.z), &sz, &cz);
    
    // XZY CW
    return TQuaternion<T>(
        cz * sx * cy + sz * cx * sy,
        cz * cx * sy + sz * sx * cy,
        sz * cx * cy - cz * sx * sy,
        cz * cx * cy - sz * sx * sy
    );
}

template <typename T>
TQuaternion<T> TQuaternion<T>::EulerYZX(const TVector3<T>& angle){
    T sx, cx, sy, cy, sz, cz;

    SinCos(ToRadianHalf(angle.x), &sx, &cx);
    SinCos(ToRadianHalf(angle.y), &sy, &cy);
    SinCos(ToRadianHalf(angle.z), &sz, &cz);
    
    // YZX CCW
    return TQuaternion<T>(
        cz * sx * cy - sz * cx * sy,
        cz * cx * sy - sz * sx * cy,
        sz * cx * cy + cz * sx * sy,
        cz * cx * cy + sz * sx * sy
    );
}

template <typename T>
TQuaternion<T> TQuaternion<T>::EulerYXZ(const TVector3<T>& angle){
    T sx, cx, sy, cy, sz, cz;

    SinCos(ToRadianHalf(angle.x), &sx, &cx);
    SinCos(ToRadianHalf(angle.y), &sy, &cy);
    SinCos(ToRadianHalf(angle.z), &sz, &cz);
    
    // YXZ CW
    return TQuaternion<T>(
        cz * sx * cy - sz * cx * sy,
        cz * cx * sy + sz * sx * cy,
        sz * cx * cy + cz * sx * sy,
        cz * cx * cy - sz * sx * sy
    );
}

template <typename T>
TQuaternion<T> TQuaternion<T>::EulerZXY(const TVector3<T>& angle){
    T sx, cx, sy, cy, sz, cz;

    SinCos(ToRadianHalf(angle.x), &sx, &cx);
    SinCos(ToRadianHalf(angle.y), &sy, &cy);
    SinCos(ToRadianHalf(angle.z), &sz, &cz);
    
    // ZXY CCW
    return TQuaternion<T>(
        cz * sx * cy + sz * cx * sy,
        cz * cx * sy - sz * sx * cy,
        sz * cx * cy - cz * sx * sy,
        cz * cx * cy + sz * sx * sy
    );
}

template <typename T>
TQuaternion<T> TQuaternion<T>::EulerZYX(const TVector3<T>& angle){
    T sx, cx, sy, cy, sz, cz;

    SinCos(ToRadianHalf(angle.x), &sx, &cx);
    SinCos(ToRadianHalf(angle.y), &sy, &cy);
    SinCos(ToRadianHalf(angle.z), &sz, &cz);
    
    // ZYX CW
    return TQuaternion<T>(
        cz * sx * cy + sz * cx * sy,
        cz * cx * sy - sz * sx * cy,
        sz * cx * cy + cz * sx * sy,
        cz * cx * cy - sz * sx * sy
    );
}

template <typename T>
TVector3<T> TQuaternion<T>::ToEulerXYZ() const{
    // 1:X 2:Y 3:Z
    // CCW
    TVector3<T> xyz;

    T y2 = y * y;
 
    T y2z = w * x + y * z;// 0.5倍矩阵2=>3系数
    T z2z = 0.5f - (x * x + y2);// 0.5倍矩阵3=>3系数
    xyz.x = ToRadian(Atan2(z2z, y2z));
 
    T z2x = 2.0f * (x * z - w * y);// 矩阵3=>1系数
    xyz.y = ToRadian(Abs(z2x) >= 1.0f ? Copysign(PI * 0.5f, z2x) : Asin(z2x));
 
    T x2y = w * z + x * y;// 0.5倍矩阵1=>2系数
    T x2x = 0.5f - (y2 + z * z);// 0.5倍矩阵1=>1系数
    xyz.z = ToRadian(Atan2(x2x, x2y));

    return xyz;
}

template <typename T>
TVector3<T> TQuaternion<T>::ToEulerXZY() const{
    // 1:X 2:Z 3:Y
    // CW 需要将后两个角度取相反数
    TVector3<T> xzy;

    T y2 = y * y;
 
    T y2z = w * x + y * z;// 0.5倍矩阵2=>3系数
    T z2z = 0.5f - (x * x + y2);// 0.5倍矩阵3=>3系数
    xzy.x = ToRadian(Atan2(z2z, y2z));
 
    T x2y = 2.0f * (w * z + x * y);// 矩阵3=>1系数
    xzy.z = -ToRadian(Abs(x2y) >= 1.0f ? Copysign(PI * 0.5f, x2y) : Asin(x2y));
 
    T x2z = w * y + z * x;// 0.5倍矩阵1=>2系数
    T x2x = 0.5f - (y2 + z * z);// 0.5倍矩阵1=>1系数
    xzy.y = -ToRadian(Atan2(x2x, x2z));

    return xzy;
}

template <typename T>
TVector3<T> TQuaternion<T>::ToEulerYZX() const{
    // 1:Y 2:Z 3:X
    // CCW
    TVector3<T> yzx;

    T z2 = z * z;
 
    T z2x = z * x - w * y;// 0.5倍矩阵2=>3系数
    T x2x = 0.5f - (y * y + z2);// 0.5倍矩阵3=>3系数
    yzx.y = ToRadian(Atan2(x2x, z2x));
 
    T x2y = 2.0f * (w * z + x * y);// 矩阵3=>1系数
    yzx.z = ToRadian(Abs(x2y) >= 1.0f ? Copysign(PI * 0.5f, x2y) : Asin(x2y));
 
    T y2z = w * x + y * z;// 0.5倍矩阵1=>2系数
    T y2y = 0.5f - (z2 + x * x);// 0.5倍矩阵1=>1系数
    yzx.x = ToRadian(Atan2(y2y, y2z));

    return yzx;
}

template <typename T>
TVector3<T> TQuaternion<T>::ToEulerYXZ() const{
    // 1:Y 2:X 3:Z
    // CW 需要将后两个角度取相反数
    TVector3<T> yxz;

    T x2 = x * x;
 
    T x2z = w * y + z * x;// 0.5倍矩阵2=>3系数
    T z2z = 0.5f - (x2 + y * y);// 0.5倍矩阵3=>3系数
    yxz.y = ToRadian(Atan2(z2z, x2z));
 
    T z2y = 2.0f * (y * z - w * x);// 矩阵3=>1系数
    yxz.x = -ToRadian(Abs(z2y) >= 1.0f ? Copysign(PI * 0.5f, z2y) : Asin(z2y));
 
    T y2x = x * y - w * z;// 0.5倍矩阵1=>2系数
    T y2y = 0.5f - (z * z + x2);// 0.5倍矩阵1=>1系数
    yxz.z = -ToRadian(Atan2(y2y, y2x));

    return yxz;
}

template <typename T>
TVector3<T> TQuaternion<T>::ToEulerZXY() const{
    // 1:Z 2:X 3:Y
    // CCW
    TVector3<T> zxy;

    T x2 = x * x;
 
    T x2y = w * z + x * y;// 0.5倍矩阵2=>3系数
    T y2y = 0.5f - (z * z + x2);// 0.5倍矩阵3=>3系数
    zxy.z = ToRadian(Atan2(y2y, x2y));
 
    T y2z = 2.0f * (w * x + y * z);// 矩阵3=>1系数
    zxy.x = ToRadian(Abs(y2z) >= 1.0f ? Copysign(PI * 0.5f, y2z) : Asin(y2z));
 
    T z2x = z * x - w * y;// 0.5倍矩阵1=>2系数
    T z2z = 0.5f - (x2 + y * y);// 0.5倍矩阵1=>1系数
    zxy.y = ToRadian(Atan2(z2z, z2x));

    return zxy;
}

template <typename T>
TVector3<T> TQuaternion<T>::ToEulerZYX() const{
    // 1:Z 2:Y 3:X
    // CW 需要将后两个角度取相反数
    TVector3<T> zyx;

    T x2 = x * x;
 
    T y2x = x * y - w * z;// 0.5倍矩阵2=>3系数
    T x2x = 0.5f - (y * y + z * z);// 0.5倍矩阵3=>3系数
    zyx.z = ToRadian(Atan2(x2x, y2x));
 
    T x2z = 2.0f * (w * y + z * x);// 矩阵3=>1系数
    zyx.y = -ToRadian(Abs(x2z) >= 1.0f ? Copysign(PI * 0.5f, x2z) : Asin(x2z));
 
    T z2y = y * z - w * x;// 0.5倍矩阵1=>2系数
    T z2z = 0.5f - (x * x + y * y);// 0.5倍矩阵1=>1系数
    zyx.x = -ToRadian(Atan2(z2z, z2y));

    return zyx;
}

template <typename T>
TMatrix4x4<T>::TMatrix4x4(){}

template <typename T>
TMatrix4x4<T>::TMatrix4x4(TQuaternion<T> &&q){
    T x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
    T xy = q.x * q.y, yz = q.y * q.z, zw = q.z * q.w;
    T xz = q.x * q.z, yw = q.y * q.w, xw = q.x * q.w;
    _11 = 1.0f - 2.0f * (y2 + z2); _12 = 2.0f * (xy - zw); _13 = 2.0f * (xz - yw); _14 = 0.0f;
    _21 = 2.0f * (xy + zw); _22 = 1.0f - 2.0f * (z2 + x2); _23 = 2.0f * (yz - xw); _24 = 0.0f;
    _31 = 2.0f * (xz + yw); _32 = 2.0f * (yz + xw); _33 = 1.0f - 2.0f * (x2 + y2); _34 = 0.0f;
    _41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

template <typename T>
TMatrix4x4<T>::TMatrix4x4(const TQuaternion<T> &q){
    T x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
    T xy = q.x * q.y, yz = q.y * q.z, zw = q.z * q.w;
    T xz = q.x * q.z, yw = q.y * q.w, xw = q.x * q.w;
    _11 = 1.0f - 2.0f * (y2 + z2); _12 = 2.0f * (xy - zw); _13 = 2.0f * (xz - yw); _14 = 0.0f;
    _21 = 2.0f * (xy + zw); _22 = 1.0f - 2.0f * (z2 + x2); _23 = 2.0f * (yz - xw); _24 = 0.0f;
    _31 = 2.0f * (xz + yw); _32 = 2.0f * (yz + xw); _33 = 1.0f - 2.0f * (x2 + y2); _34 = 0.0f;
    _41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

template <typename T>
TMatrix4x4<T> &TMatrix4x4<T>::operator=(TQuaternion<T> &&q){
    T x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
    T xy = q.x * q.y, yz = q.y * q.z, zw = q.z * q.w;
    T xz = q.x * q.z, yw = q.y * q.w, xw = q.x * q.w;
    _11 = 1.0f - 2.0f * (y2 + z2); _12 = 2.0f * (xy - zw); _13 = 2.0f * (xz - yw); _14 = 0.0f;
    _21 = 2.0f * (xy + zw); _22 = 1.0f - 2.0f * (z2 + x2); _23 = 2.0f * (yz - xw); _24 = 0.0f;
    _31 = 2.0f * (xz + yw); _32 = 2.0f * (yz + xw); _33 = 1.0f - 2.0f * (x2 + y2); _34 = 0.0f;
    _41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
    return *this;
}

template <typename T>
TMatrix4x4<T> &TMatrix4x4<T>::operator=(const TQuaternion<T> &q){
    T x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
    T xy = q.x * q.y, yz = q.y * q.z, zw = q.z * q.w;
    T xz = q.x * q.z, yw = q.y * q.w, xw = q.x * q.w;
    _11 = 1.0f - 2.0f * (y2 + z2); _12 = 2.0f * (xy - zw); _13 = 2.0f * (xz - yw); _14 = 0.0f;
    _21 = 2.0f * (xy + zw); _22 = 1.0f - 2.0f * (z2 + x2); _23 = 2.0f * (yz - xw); _24 = 0.0f;
    _31 = 2.0f * (xz + yw); _32 = 2.0f * (yz + xw); _33 = 1.0f - 2.0f * (x2 + y2); _34 = 0.0f;
    _41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
    return *this;
}

template <typename T>
constexpr TMatrix4x4<T>::TMatrix4x4(T x) :
                     _11(x), _12(0.0f), _13(0.0f), _14(0.0f),
                     _21(0.0f), _22(x), _23(0.0f), _24(0.0f),
                     _31(0.0f), _32(0.0f), _33(x), _34(0.0f),
                     _41(0.0f), _42(0.0f), _43(0.0f), _44(x) {}

template <typename T>
constexpr TMatrix4x4<T>::TMatrix4x4(T _11, T _12, T _13, T _14,
                                    T _21, T _22, T _23, T _24,
                                    T _31, T _32, T _33, T _34,
                                    T _41, T _42, T _43, T _44) :
                                    _11(_11), _12(_12), _13(_13), _14(_14),
                                    _21(_21), _22(_22), _23(_23), _24(_24),
                                    _31(_31), _32(_32), _33(_33), _34(_34),
                                    _41(_41), _42(_42), _43(_43), _44(_44) {}

template <typename T>
const TMatrix4x4<T> TMatrix4x4<T>::zero      = {0.0F, 0.0F, 0.0F, 0.0F,
                                                0.0F, 0.0F, 0.0F, 0.0F,
                                                0.0F, 0.0F, 0.0F, 0.0F,
                                                0.0F, 0.0F, 0.0F, 0.0F};
template <typename T>
const TMatrix4x4<T> TMatrix4x4<T>::identity  = {1.0F, 0.0F, 0.0F, 0.0F,
                                                0.0F, 1.0F, 0.0F, 0.0F,
                                                0.0F, 0.0F, 1.0F, 0.0F,
                                                0.0F, 0.0F, 0.0F, 1.0F};

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::operator+(const TMatrix4x4<T>& m) const{
    return {_11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14,
            _21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24,
            _31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34,
            _41 + m._41, _42 + m._42, _43 + m._43, _44 + m._44};
}

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::operator-(const TMatrix4x4<T>& m) const{
    return {_11 - m._11, _12 - m._12, _13 - m._13, _14 - m._14,
            _21 - m._21, _22 - m._22, _23 - m._23, _24 - m._24,
            _31 - m._31, _32 - m._32, _33 - m._33, _34 - m._34,
            _41 - m._41, _42 - m._42, _43 - m._43, _44 - m._44};
}

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::operator*(T w) const{
    return {_11 * w, _12 * w, _13 * w, _14 * w,
            _21 * w, _22 * w, _23 * w, _24 * w,
            _31 * w, _32 * w, _33 * w, _34 * w,
            _41 * w, _42 * w, _43 * w, _44 * w};
}

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::operator/(T w) const{
    w = 1.0F / w;
    return {_11 * w, _12 * w, _13 * w, _14 * w,
            _21 * w, _22 * w, _23 * w, _24 * w,
            _31 * w, _32 * w, _33 * w, _34 * w,
            _41 * w, _42 * w, _43 * w, _44 * w};
}

template <>
TMatrix4x4<int> TMatrix4x4<int>::operator/(int w) const;

template <typename T>
TMatrix4x4<T> &TMatrix4x4<T>::operator+=(const TMatrix4x4<T>& m){
    _11 += m._11; _12 += m._12; _13 += m._13; _14 += m._14;
    _21 += m._21; _22 += m._22; _23 += m._23; _24 += m._24;
    _31 += m._31; _32 += m._32; _33 += m._33; _34 += m._34;
    _41 += m._41; _42 += m._42; _43 += m._43; _44 += m._44;
    return *this;
}

template <typename T>
TMatrix4x4<T> &TMatrix4x4<T>::operator-=(const TMatrix4x4<T>& m){
    _11 -= m._11; _12 -= m._12; _13 -= m._13; _14 -= m._14;
    _21 -= m._21; _22 -= m._22; _23 -= m._23; _24 -= m._24;
    _31 -= m._31; _32 -= m._32; _33 -= m._33; _34 -= m._34;
    _41 -= m._41; _42 -= m._42; _43 -= m._43; _44 -= m._44;
    return *this;
}

template <typename T>
TMatrix4x4<T> &TMatrix4x4<T>::operator*=(T w){
    _11 *= w; _12 *= w; _13 *= w; _14 *= w;
    _21 *= w; _22 *= w; _23 *= w; _24 *= w;
    _31 *= w; _32 *= w; _33 *= w; _34 *= w;
    _41 *= w; _42 *= w; _43 *= w; _44 *= w;
    return *this;
}

template <typename T>
TMatrix4x4<T> &TMatrix4x4<T>::operator/=(T w){
    w = 1.0F / w;
    _11 *= w; _12 *= w; _13 *= w; _14 *= w;
    _21 *= w; _22 *= w; _23 *= w; _24 *= w;
    _31 *= w; _32 *= w; _33 *= w; _34 *= w;
    _41 *= w; _42 *= w; _43 *= w; _44 *= w;
    return *this;
}

template <>
TMatrix4x4<int> &TMatrix4x4<int>::operator/=(int w);

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::operator*(const TMatrix4x4<T>& m) const{
    return {_11 * m._11 + _12 * m._21 + _13 * m._31 + _14 * m._41,
            _11 * m._12 + _12 * m._22 + _13 * m._32 + _14 * m._42,
            _11 * m._13 + _12 * m._23 + _13 * m._33 + _14 * m._43,
            _11 * m._14 + _12 * m._24 + _13 * m._34 + _14 * m._44,
            _21 * m._11 + _22 * m._21 + _23 * m._31 + _24 * m._41,
            _21 * m._12 + _22 * m._22 + _23 * m._32 + _24 * m._42,
            _21 * m._13 + _22 * m._23 + _23 * m._33 + _24 * m._43,
            _21 * m._14 + _22 * m._24 + _23 * m._34 + _24 * m._44,
            _31 * m._11 + _32 * m._21 + _33 * m._31 + _34 * m._41,
            _31 * m._12 + _32 * m._22 + _33 * m._32 + _34 * m._42,
            _31 * m._13 + _32 * m._23 + _33 * m._33 + _34 * m._43,
            _31 * m._14 + _32 * m._24 + _33 * m._34 + _34 * m._44,
            _41 * m._11 + _42 * m._21 + _43 * m._31 + _44 * m._41,
            _41 * m._12 + _42 * m._22 + _43 * m._32 + _44 * m._42,
            _41 * m._13 + _42 * m._23 + _43 * m._33 + _44 * m._43,
            _41 * m._14 + _42 * m._24 + _43 * m._34 + _44 * m._44};
}

template <typename T>
TMatrix4x4<T> &TMatrix4x4<T>::operator*=(const TMatrix4x4<T>& m){
    // 将矩阵视作变换，*=操作是加上变换的意思，对于矩阵来说应为左乘
    return (*this = m * *this);
}

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::operator-() const{
    return {-_11, -_12, -_13, -_14,
            -_21, -_22, -_23, -_24,
            -_31, -_32, -_33, -_34,
            -_41, -_42, -_43, -_44};
}

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::operator~() const{
    return {_11, _21, _31, _41,
            _12, _22, _32, _42,
            _13, _23, _33, _43,
            _14, _24, _34, _44};
}

template <typename T>
TVector3<T> TMatrix4x4<T>::operator*(const TVector2<T>& v) const{
    return TVector3<T>(
        _11 * v.x + _12 * v.y,
        _21 * v.x + _22 * v.y,
        _31 * v.x + _32 * v.y
    );
}

template <typename T>
TPoint3<T> TMatrix4x4<T>::operator*(const TPoint2<T>& p) const{
    return TPoint3<T>(
        _11 * p.x + _12 * p.y + _14,
        _21 * p.x + _22 * p.y + _24,
        _31 * p.x + _32 * p.y + _34
    );
}

template <typename T>
TVector3<T> TMatrix4x4<T>::operator*(const TVector3<T>& v) const{
    return TVector3<T>(
        _11 * v.x + _12 * v.y + _13 * v.z,
        _21 * v.x + _22 * v.y + _23 * v.z,
        _31 * v.x + _32 * v.y + _33 * v.z
    );
}

template <typename T>
TPoint3<T> TMatrix4x4<T>::operator*(const TPoint3<T>& p) const{
    return TPoint3<T>(
        _11 * p.x + _12 * p.y + _13 * p.z + _14,
        _21 * p.x + _22 * p.y + _23 * p.z + _24,
        _31 * p.x + _32 * p.y + _33 * p.z + _34
    );
}

template <typename T>
TVector4<T> TMatrix4x4<T>::operator*(const TVector4<T>& v) const{
    return TVector4<T>(
        _11 * v.x + _12 * v.y + _13 * v.z + _14 * v.w,
        _21 * v.x + _22 * v.y + _23 * v.z + _24 * v.w,
        _31 * v.x + _32 * v.y + _33 * v.z + _34 * v.w,
        _41 * v.x + _42 * v.y + _43 * v.z + _44 * v.w
    );
}

template <typename T>
T TMatrix4x4<T>::Determinant() const{
    T det3434 = _33 * _44 - _34 * _43;
    T det2434 = _23 * _44 - _24 * _43;
    T det2334 = _23 * _34 - _24 * _33;
    T det3424 = _32 * _44 - _34 * _42;
    T det2424 = _22 * _44 - _24 * _42;
    T det2324 = _22 * _34 - _24 * _32;
    T det3423 = _32 * _43 - _33 * _42;
    T det2423 = _22 * _43 - _23 * _42;
    T det2323 = _22 * _33 - _23 * _32;

    T res =
        + _11 * (+ _22 * det3434 - _32 * det2434 + _42 * det2334)
        + _21 * (- _21 * det3434 + _31 * det2434 - _41 * det2334)
        + _31 * (+ _21 * det3424 - _31 * det2424 + _41 * det2324)
        + _41 * (- _21 * det3423 + _31 * det2423 - _41 * det2323);
    
    return res;
}

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::Inverse() const{
    T num00 = _33 * _44 - _34 * _43;
    T num01 = _23 * _44 - _24 * _43;
    T num02 = _23 * _34 - _24 * _33;
    T num03 = _13 * _44 - _14 * _43;
    T num04 = _13 * _34 - _14 * _33;
    T num05 = _13 * _24 - _14 * _23;
    T num06 = _32 * _44 - _34 * _42;
    T num07 = _22 * _44 - _24 * _42;
    T num08 = _22 * _34 - _24 * _32;
    T num09 = _12 * _44 - _14 * _42;
    T num10 = _12 * _34 - _14 * _32;
    T num11 = _22 * _44 - _24 * _42;
    T num12 = _12 * _24 - _14 * _22;
    T num13 = _32 * _43 - _33 * _42;
    T num14 = _22 * _43 - _23 * _42;
    T num15 = _22 * _33 - _23 * _32;
    T num16 = _12 * _43 - _13 * _42;
    T num17 = _12 * _33 - _13 * _32;
    T num18 = _12 * _23 - _13 * _22;

    TMatrix4x4<T> res(
        + _22 * num00 - _32 * num01 + _42 * num02,
        - _21 * num00 + _31 * num01 - _41 * num02,
        + _21 * num06 - _31 * num07 + _41 * num08,
        - _21 * num13 + _31 * num14 - _41 * num15,

        - _12 * num00 + _32 * num03 - _42 * num04,
        + _11 * num00 - _31 * num03 + _41 * num04,
        - _11 * num06 + _31 * num09 - _41 * num10,
        + _11 * num13 - _31 * num16 + _41 * num17,

        + _12 * num01 - _22 * num03 + _42 * num05,
        - _11 * num01 + _21 * num03 - _41 * num05,
        + _11 * num11 - _21 * num09 + _41 * num12,
        - _11 * num14 + _21 * num16 - _41 * num18,

        - _12 * num02 + _22 * num04 - _32 * num05,
        + _11 * num02 - _21 * num04 + _31 * num05,
        - _11 * num08 + _21 * num10 - _31 * num12,
        + _11 * num15 - _21 * num17 + _31 * num18
    );

    T determinant =
        + _11 * res._11
        + _21 * res._12
        + _31 * res._13
        + _41 * res._14;

    res /= determinant;
    return res;
}

template <typename T>
TMatrix4x4<T> &TMatrix4x4<T>::AddTranslation(const TVector3<T>& v){
    _14 += v.x; _24 += v.y; _34 += v.z;
    return *this;
}

template <typename T>
TMatrix4x4<T> &TMatrix4x4<T>::AddScale(const TVector3<T>& v){
    _11 *= v.x; _12 *= v.x; _13 *= v.x;
    _21 *= v.y; _22 *= v.y; _23 *= v.y;
    _31 *= v.z; _32 *= v.z; _33 *= v.z;
    return *this;
}

template <typename T>
TVector3<T> TMatrix4x4<T>::GetTranslation() const{
    return TVector3<T>(_14, _24, _34);
}

template <typename T>
TQuaternion<T> TMatrix4x4<T>::GetRotation() const{
    TVector3<T> forward = TVector3<T>(_12, _22, _32);
    TVector3<T> up = TVector3<T>(_13, _23, _33);
    return TQuaternion<T>::LookAt(forward, up);
}

template <typename T>
TVector3<T> TMatrix4x4<T>::GetScale() const{
    return TVector3<T>(
        Sqrt(_11 * _11 + _12 * _12 + _13 * _13),
        Sqrt(_21 * _21 + _22 * _22 + _23 * _23),
        Sqrt(_31 * _31 + _32 * _32 + _33 * _33)
    );
}

template <typename T>
TRect<T>::TRect(){}

template <typename T>
TRect<T>::TRect(const TVector2<T>& p1, const TVector2<T>& p2) : left(p1.x), right(p2.x), top(p1.y), bottom(p2.y) {
    Sort(left, right); Sort(bottom, top);
}

template <typename T>
TRect<T>::TRect(T left, T right, T bottom, T top)
    : left(left), right(right), bottom(bottom), top(top) {
    Sort(left, right); Sort(bottom, top);
}

template <typename T>
const TRect<T> TRect<T>::zero = {0.0F, 0.0F, 0.0F, 0.0F};

template <typename T>
T TRect<T>::GetAspect() const{
    return (right - left) / (top - bottom);
}

template <typename T>
T TRect<T>::GetWidth() const{
    return right - left;
}

template <typename T>
T TRect<T>::GetHeight() const{
    return top - bottom;
}

template <typename T>
TVector2<T> TRect<T>::GetSize() const{
    return TVector2<T>(right - left, top - bottom);
}

template <typename T>
bool TRect<T>::Inside(const TVector2<T>& pos) const{
    return pos.x >= left && pos.x <= right &&
            pos.y >= bottom && pos.y <= top;
}

template <typename T>
T TRect<T>::GetXRatio(T x) const{
    return GetRate(x, left, right);
}

template <typename T>
T TRect<T>::GetYRatio(T y) const{
    return GetRate(y, bottom, top);
}

template <typename T>
TVector2<T> TRect<T>::GetRatio(T x, T y) const{
    return TVector2<T>(GetRate(x, left, right), GetRate(y, bottom, top));
}

template <typename T>
TVector2<T> TRect<T>::GetRatio(const TVector2<T>& pos) const{
    return GetRatio(pos.x, pos.y);
}

template <typename T>
T TRect<T>::GetXRatioPos(T ratio) const{
    return Lerp(left, right, ratio);
}

template <typename T>
T TRect<T>::GetYRatioPos(T ratio) const{
    return Lerp(bottom, top, ratio);
}

template <typename T>
TVector2<T> TRect<T>::GetRatioPos(T ratioX, T ratioY) const{
    return TVector2<T>(Lerp(left, right, ratioX), Lerp(bottom, top, ratioY));
}

template <typename T>
TVector2<T> TRect<T>::GetRatioPos(const TVector2<T>& ratio) const{
    return GetRatioPos(ratio.x, ratio.y);
}

template <typename T>
T TRect<T>::MapXPos(const TRect<T>& rect, T x) const{
    return GetXRatioPos(rect.GetXRatio(x));
}

template <typename T>
T TRect<T>::MapYPos(const TRect<T>& rect, T y) const{
    return GetYRatioPos(rect.GetYRatio(y));
}

template <typename T>
TVector2<T> TRect<T>::MapPos(const TRect<T>& rect, const TVector2<T>& pos) const{
    return GetRatioPos(rect.GetRatio(pos));
}

template <typename T>
TVector2<T> TRect<T>::MapPos(const TRect<T>& rect, T x, T y) const{
    return GetRatioPos(rect.GetRatio(x, y));
}

template <typename T>
TRect<T> TRect<T>::ChildRect(const TRect<T>& ratio) const{
    return ChildRect(ratio.left, ratio.right, ratio.bottom, ratio.top);
}

template <typename T>
TRect<T> TRect<T>::ChildRect(T left, T right, T bottom, T top) const{
    TRect<T> rect;
    TVector2<T> size;

    size = GetSize();

    rect.left = this->left + Round(size.x * Clamp((left + 1.0f) * 0.5f, 0.0f, 1.0f));
    rect.right = this->left + Round(size.x * Clamp((right + 1.0f) * 0.5f, 0.0f, 1.0f));
    rect.bottom = this->bottom + Round(size.y * Clamp((bottom + 1.0f) * 0.5f, 0.0f, 1.0f));
    rect.top = this->bottom + Round(size.y * Clamp((top + 1.0f) * 0.5f, 0.0f, 1.0f));

    return rect;
}

}

#endif