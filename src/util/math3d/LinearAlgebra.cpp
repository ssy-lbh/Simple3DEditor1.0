#include <util/math3d/LinearAlgebra.h>

#include <util/math3d/Math.h>

// 可能是因为一旦全特化，编译器直接将函数汇编输出
// 所以这里将声明和实现分离
template <>
TVector2<int> TVector2<int>::operator/(int w) const{
    return TVector2<int>(x / w, y / w);
}

template <>
TVector2<int> &TVector2<int>::operator/=(int w){
    x /= w; y /= w;
    return *this;
}

template <>
TVector3<int> TVector3<int>::operator/(int w) const{
    return TVector3<int>(x / w, y / w, z / w);
}

template <>
TVector3<int> &TVector3<int>::operator/=(int w){
    x /= w; y /= w; z /= w;
    return *this;
}

template <>
TVector4<int> TVector4<int>::operator/(int w) const{
    return TVector4<int>(x / w, y / w, z / w, this->w);
}

template <>
TVector4<int> &TVector4<int>::operator/=(int w){
    x /= w; y /= w; z /= w;
    return *this;
}

template <>
TComplex<int> operator/(const TComplex<int>& c, int x){
    return TComplex<int>(c.real / x, c.imag / x);
}

template <>
TComplex<int> &TComplex<int>::operator/=(int x){
    real /= x; imag /= x;
    return *this;
}

template <>
TQuaternion<int> TQuaternion<int>::operator/(int s) const{
    return TQuaternion<int>(x / s, y / s, z / s, w / s);
}

template <>
TQuaternion<int> &TQuaternion<int>::operator/=(int s){
    x /= s; y /= s; z /= s; w /= s;
    return *this;
}

template <>
TMatrix4x4<int> TMatrix4x4<int>::operator/(int w) const{
    return {_11 / w, _12 / w, _13 / w, _14 / w,
            _21 / w, _22 / w, _23 / w, _24 / w,
            _31 / w, _32 / w, _33 / w, _34 / w,
            _41 / w, _42 / w, _43 / w, _44 / w};
}

template <>
TMatrix4x4<int> &TMatrix4x4<int>::operator/=(int w){
    _11 /= w; _12 /= w; _13 /= w; _14 /= w;
    _21 /= w; _22 /= w; _23 /= w; _24 /= w;
    _31 /= w; _32 /= w; _33 /= w; _34 /= w;
    _41 /= w; _42 /= w; _43 /= w; _44 /= w;
    return *this;
}

Matrix3x4::Matrix3x4(){}

Matrix3x4::Matrix3x4(Quaternion &&q){
    float x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
    float xy = q.x * q.y, yz = q.y * q.z, zw = q.z * q.w;
    float xz = q.x * q.z, yw = q.y * q.w, xw = q.x * q.w;
    _11 = 1.0f - 2.0f * (y2 + z2); _12 = 2.0f * (xy - zw); _13 = 2.0f * (xz - yw); _14 = 0.0f;
    _21 = 2.0f * (xy + zw); _22 = 1.0f - 2.0f * (z2 + x2); _23 = 2.0f * (yz - xw); _24 = 0.0f;
    _31 = 2.0f * (xz + yw); _32 = 2.0f * (yz + xw); _33 = 1.0f - 2.0f * (x2 + y2); _34 = 0.0f;
}

Matrix3x4::Matrix3x4(Matrix4x4 &&m) :
    _11(m._11), _12(m._12), _13(m._13), _14(m._14),
    _21(m._21), _22(m._22), _23(m._23), _24(m._24),
    _31(m._31), _32(m._32), _33(m._33), _34(m._34) {}

Matrix3x4::Matrix3x4(const Quaternion &q){
    float x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
    float xy = q.x * q.y, yz = q.y * q.z, zw = q.z * q.w;
    float xz = q.x * q.z, yw = q.y * q.w, xw = q.x * q.w;
    _11 = 1.0f - 2.0f * (y2 + z2); _12 = 2.0f * (xy - zw); _13 = 2.0f * (xz - yw); _14 = 0.0f;
    _21 = 2.0f * (xy + zw); _22 = 1.0f - 2.0f * (z2 + x2); _23 = 2.0f * (yz - xw); _24 = 0.0f;
    _31 = 2.0f * (xz + yw); _32 = 2.0f * (yz + xw); _33 = 1.0f - 2.0f * (x2 + y2); _34 = 0.0f;
}

Matrix3x4::Matrix3x4(const Matrix4x4 &m) :
    _11(m._11), _12(m._12), _13(m._13), _14(m._14),
    _21(m._21), _22(m._22), _23(m._23), _24(m._24),
    _31(m._31), _32(m._32), _33(m._33), _34(m._34) {}

Matrix3x4 &Matrix3x4::operator=(Quaternion &&q){
    float x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
    float xy = q.x * q.y, yz = q.y * q.z, zw = q.z * q.w;
    float xz = q.x * q.z, yw = q.y * q.w, xw = q.x * q.w;
    _11 = 1.0f - 2.0f * (y2 + z2); _12 = 2.0f * (xy - zw); _13 = 2.0f * (xz - yw); _14 = 0.0f;
    _21 = 2.0f * (xy + zw); _22 = 1.0f - 2.0f * (z2 + x2); _23 = 2.0f * (yz - xw); _24 = 0.0f;
    _31 = 2.0f * (xz + yw); _32 = 2.0f * (yz + xw); _33 = 1.0f - 2.0f * (x2 + y2); _34 = 0.0f;
    return *this;
}

Matrix3x4 &Matrix3x4::operator=(Matrix4x4 &&m){
    _11 = m._11; _12 = m._12; _13 = m._13; _14 = m._14;
    _21 = m._21; _22 = m._22; _23 = m._23; _24 = m._24;
    _31 = m._31; _32 = m._32; _33 = m._33; _34 = m._34;
    return *this;
}

Matrix3x4 &Matrix3x4::operator=(const Quaternion &q){
    float x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
    float xy = q.x * q.y, yz = q.y * q.z, zw = q.z * q.w;
    float xz = q.x * q.z, yw = q.y * q.w, xw = q.x * q.w;
    _11 = 1.0f - 2.0f * (y2 + z2); _12 = 2.0f * (xy - zw); _13 = 2.0f * (xz - yw); _14 = 0.0f;
    _21 = 2.0f * (xy + zw); _22 = 1.0f - 2.0f * (z2 + x2); _23 = 2.0f * (yz - xw); _24 = 0.0f;
    _31 = 2.0f * (xz + yw); _32 = 2.0f * (yz + xw); _33 = 1.0f - 2.0f * (x2 + y2); _34 = 0.0f;
    return *this;
}

Matrix3x4 &Matrix3x4::operator=(const Matrix4x4 &m){
    _11 = m._11; _12 = m._12; _13 = m._13; _14 = m._14;
    _21 = m._21; _22 = m._22; _23 = m._23; _24 = m._24;
    _31 = m._31; _32 = m._32; _33 = m._33; _34 = m._34;
    return *this;
}

Matrix3x4::Matrix3x4(float _11, float _12, float _13, float _14,
                     float _21, float _22, float _23, float _24,
                     float _31, float _32, float _33, float _34) :
                     _11(_11), _12(_12), _13(_13), _14(_14),
                     _21(_21), _22(_22), _23(_23), _24(_24),
                     _31(_31), _32(_32), _33(_33), _34(_34) {}

const Matrix3x4 Matrix3x4::zero      = {0.0F, 0.0F, 0.0F, 0.0F,
                                        0.0F, 0.0F, 0.0F, 0.0F,
                                        0.0F, 0.0F, 0.0F, 0.0F};
const Matrix3x4 Matrix3x4::identity  = {1.0F, 0.0F, 0.0F, 0.0F,
                                        0.0F, 1.0F, 0.0F, 0.0F,
                                        0.0F, 0.0F, 1.0F, 0.0F};

Matrix3x4 Matrix3x4::operator+(Matrix3x4 m) const{
    return {_11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14,
            _21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24,
            _31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34};
}

Matrix3x4 Matrix3x4::operator-(Matrix3x4 m) const{
    return {_11 - m._11, _12 - m._12, _13 - m._13, _14 - m._14,
            _21 - m._21, _22 - m._22, _23 - m._23, _24 - m._24,
            _31 - m._31, _32 - m._32, _33 - m._33, _34 - m._34};
}

Matrix3x4 Matrix3x4::operator*(float w) const{
    return {_11 * w, _12 * w, _13 * w, _14 * w,
            _21 * w, _22 * w, _23 * w, _24 * w,
            _31 * w, _32 * w, _33 * w, _34 * w};
}

Matrix3x4 Matrix3x4::operator/(float w) const{
    w = 1.0F / w;
    return {_11 * w, _12 * w, _13 * w, _14 * w,
            _21 * w, _22 * w, _23 * w, _24 * w,
            _31 * w, _32 * w, _33 * w, _34 * w};
}

Matrix3x4 &Matrix3x4::operator+=(Matrix3x4 m){
    _11 += m._11; _12 += m._12; _13 += m._13; _14 += m._14;
    _21 += m._21; _22 += m._22; _23 += m._23; _24 += m._24;
    _31 += m._31; _32 += m._32; _33 += m._33; _34 += m._34;
    return *this;
}

Matrix3x4 &Matrix3x4::operator-=(Matrix3x4 m){
    _11 -= m._11; _12 -= m._12; _13 -= m._13; _14 -= m._14;
    _21 -= m._21; _22 -= m._22; _23 -= m._23; _24 -= m._24;
    _31 -= m._31; _32 -= m._32; _33 -= m._33; _34 -= m._34;
    return *this;
}

Matrix3x4 &Matrix3x4::operator*=(float w){
    _11 *= w; _12 *= w; _13 *= w; _14 *= w;
    _21 *= w; _22 *= w; _23 *= w; _24 *= w;
    _31 *= w; _32 *= w; _33 *= w; _34 *= w;
    return *this;
}

Matrix3x4 &Matrix3x4::operator/=(float w){
    w = 1.0F / w;
    _11 *= w; _12 *= w; _13 *= w; _14 *= w;
    _21 *= w; _22 *= w; _23 *= w; _24 *= w;
    _31 *= w; _32 *= w; _33 *= w; _34 *= w;
    return *this;
}

Matrix3x4 Matrix3x4::operator*(Matrix3x4 m) const{
    return {_11 * m._11 + _12 * m._21 + _13 * m._31,
            _11 * m._12 + _12 * m._22 + _13 * m._32,
            _11 * m._13 + _12 * m._23 + _13 * m._33,
            _11 * m._14 + _12 * m._24 + _13 * m._34 + _14,
            _21 * m._11 + _22 * m._21 + _23 * m._31,
            _21 * m._12 + _22 * m._22 + _23 * m._32,
            _21 * m._13 + _22 * m._23 + _23 * m._33,
            _21 * m._14 + _22 * m._24 + _23 * m._34 + _24,
            _31 * m._11 + _32 * m._21 + _33 * m._31,
            _31 * m._12 + _32 * m._22 + _33 * m._32,
            _31 * m._13 + _32 * m._23 + _33 * m._33,
            _31 * m._14 + _32 * m._24 + _33 * m._34 + _34};
}

Matrix3x4 &Matrix3x4::operator*=(Matrix3x4 m){
    return (*this = m * *this);
}

Matrix3x4 Matrix3x4::operator-() const{
    return {-_11, -_12, -_13, -_14,
            -_21, -_22, -_23, -_24,
            -_31, -_32, -_33, -_34};
}

Vector3 Matrix3x4::operator*(Vector2 v) const{
    return Vector3(
        _11 * v.x + _12 * v.y,
        _21 * v.x + _22 * v.y,
        _31 * v.x + _32 * v.y
    );
}

Point3 Matrix3x4::operator*(Point2 p) const{
    return Point3(
        _11 * p.x + _12 * p.y + _14,
        _21 * p.x + _22 * p.y + _24,
        _31 * p.x + _32 * p.y + _34
    );
}

Vector3 Matrix3x4::operator*(Vector3 v) const{
    return Vector3(
        _11 * v.x + _12 * v.y + _13 * v.z,
        _21 * v.x + _22 * v.y + _23 * v.z,
        _31 * v.x + _32 * v.y + _33 * v.z
    );
}

Point3 Matrix3x4::operator*(Point3 p) const{
    return Point3(
        _11 * p.x + _12 * p.y + _13 * p.z + _14,
        _21 * p.x + _22 * p.y + _23 * p.z + _24,
        _31 * p.x + _32 * p.y + _33 * p.z + _34
    );
}

Vector4 Matrix3x4::operator*(Vector4 v) const{
    return Vector4(
        _11 * v.x + _12 * v.y + _13 * v.z + _14 * v.w,
        _21 * v.x + _22 * v.y + _23 * v.z + _24 * v.w,
        _31 * v.x + _32 * v.y + _33 * v.z + _34 * v.w,
        v.w
    );
}

Matrix3x4 &Matrix3x4::AddTranslation(Vector3 v){
    _14 += v.x; _24 += v.y; _34 += v.z;
    return *this;
}

Matrix3x4 &Matrix3x4::AddScale(Vector3 v){
    _11 *= v.x; _12 *= v.x; _13 *= v.x;
    _21 *= v.y; _22 *= v.y; _23 *= v.y;
    _31 *= v.z; _32 *= v.z; _33 *= v.z;
    return *this;
}

Vector3 Matrix3x4::GetTranslation() const{
    return Vector3(_14, _24, _34);
}

Quaternion Matrix3x4::GetRotation() const{
    Vector3 forward = Vector3(_12, _22, _32);
    Vector3 up = Vector3(_13, _23, _33);
    return Quaternion::LookAt(forward, up);
}

Vector3 Matrix3x4::GetScale() const{
    return Vector3(
        Sqrt(_11 * _11 + _12 * _12 + _13 * _13),
        Sqrt(_21 * _21 + _22 * _22 + _23 * _23),
        Sqrt(_31 * _31 + _32 * _32 + _33 * _33)
    );
}

Matrix2x3::Matrix2x3(){}

Matrix2x3::Matrix2x3(Complex &&c){
    _11 = c.real; _12 = -c.imag; _13 = 0.0f;
    _21 = c.imag; _22 = c.real; _23 = 0.0f;
}

Matrix2x3::Matrix2x3(const Complex &c){
    _11 = c.real; _12 = -c.imag; _13 = 0.0f;
    _21 = c.imag; _22 = c.real; _23 = 0.0f;
}

Matrix2x3 &Matrix2x3::operator=(Complex &&c){
    _11 = c.real; _12 = -c.imag; _13 = 0.0f;
    _21 = c.imag; _22 = c.real; _23 = 0.0f;
    return *this;
}

Matrix2x3 &Matrix2x3::operator=(const Complex &c){
    _11 = c.real; _12 = -c.imag; _13 = 0.0f;
    _21 = c.imag; _22 = c.real; _23 = 0.0f;
    return *this;
}

Matrix2x3::Matrix2x3(float _11, float _12, float _13,
                     float _21, float _22, float _23) :
                     _11(_11), _12(_12), _13(_13),
                     _21(_21), _22(_22), _23(_23) {}

const Matrix2x3 Matrix2x3::zero      = {0.0F, 0.0F, 0.0F,
                                        0.0F, 0.0F, 0.0F};
const Matrix2x3 Matrix2x3::identity  = {1.0F, 0.0F, 0.0F,
                                        0.0F, 1.0F, 0.0F};

Matrix2x3 Matrix2x3::operator+(Matrix2x3 m) const{
    return {_11 + m._11, _12 + m._12, _13 + m._13,
            _21 + m._21, _22 + m._22, _23 + m._23};
}

Matrix2x3 Matrix2x3::operator-(Matrix2x3 m) const{
    return {_11 - m._11, _12 - m._12, _13 - m._13,
            _21 - m._21, _22 - m._22, _23 - m._23};
}

Matrix2x3 Matrix2x3::operator*(float w) const{
    return {_11 * w, _12 * w, _13 * w,
            _21 * w, _22 * w, _23 * w};

}

Matrix2x3 Matrix2x3::operator/(float w) const{
    w = 1.0F / w;
    return {_11 * w, _12 * w, _13 * w,
            _21 * w, _22 * w, _23 * w};
}

Matrix2x3 &Matrix2x3::operator+=(Matrix2x3 m){
    _11 += m._11; _12 += m._12; _13 += m._13;
    _21 += m._21; _22 += m._22; _23 += m._23;
    return *this;
}

Matrix2x3 &Matrix2x3::operator-=(Matrix2x3 m){
    _11 -= m._11; _12 -= m._12; _13 -= m._13;
    _21 -= m._21; _22 -= m._22; _23 -= m._23;
    return *this;
}

Matrix2x3 &Matrix2x3::operator*=(float w){
    _11 *= w; _12 *= w; _13 *= w;
    _21 *= w; _22 *= w; _23 *= w;
    return *this;
}

Matrix2x3 &Matrix2x3::operator/=(float w){
    w = 1.0F / w;
    _11 *= w; _12 *= w; _13 *= w;
    _21 *= w; _22 *= w; _23 *= w;
    return *this;
}

Matrix2x3 Matrix2x3::operator*(Matrix2x3 m) const{
    return {_11 * m._11 + _12 * m._21,
            _11 * m._12 + _12 * m._22,
            _11 * m._13 + _12 * m._23 + _13,
            _21 * m._11 + _22 * m._21,
            _21 * m._12 + _22 * m._22,
            _21 * m._13 + _22 * m._23 + _23};
}

Matrix2x3 &Matrix2x3::operator*=(Matrix2x3 m){
    return (*this = m * *this);
}

Matrix2x3 Matrix2x3::operator-() const{
    return {-_11, -_12, -_13,
            -_21, -_22, -_23};
}

Vector2 Matrix2x3::operator*(Vector2 v) const{
    return Vector2(
        _11 * v.x + _12 * v.y,
        _21 * v.x + _22 * v.y
    );
}

Point2 Matrix2x3::operator*(Point2 p) const{
    return Point2(
        _11 * p.x + _12 * p.y + _13,
        _21 * p.x + _22 * p.y + _23
    );
}

Matrix2x3 &Matrix2x3::AddTranslation(Vector2 v){
    _13 += v.x; _23 += v.y;
    return *this;
}

Matrix2x3 &Matrix2x3::AddScale(Vector2 v){
    _11 *= v.x; _12 *= v.x;
    _21 *= v.y; _22 *= v.y;
    return *this;
}

Vector2 Matrix2x3::GetTranslation() const{
    return Vector2(_13, _23);
}

Complex Matrix2x3::GetRotation() const{
    return Complex(_11, _21).Normal();
}

Vector2 Matrix2x3::GetScale() const{
    return Vector2(
        Sqrt(_11 * _11 + _12 * _12),
        Sqrt(_21 * _21 + _22 * _22)
    );
}
