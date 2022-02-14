#include "vecmath.h"

Vector2::Vector2() : x(0.0f), y(0.0f){}

Vector2::Vector2(Vector3 &&v) : x(v.x), y(v.y){}

Vector2::Vector2(Vector4 &&v) : x(v.x), y(v.y){}

Vector2::Vector2(const Vector3 &v) : x(v.x), y(v.y){}

Vector2::Vector2(const Vector4 &v) : x(v.x), y(v.y){}

Vector2 &Vector2::operator=(Vector3 &&v){
    x = v.x; y = v.y;
    return *this;
}

Vector2 &Vector2::operator=(Vector4 &&v){
    x = v.x; y = v.y;
    return *this;
}

Vector2 &Vector2::operator=(const Vector3 &v){
    x = v.x; y = v.y;
    return *this;
}

Vector2 &Vector2::operator=(const Vector4 &v){
    x = v.x; y = v.y;
    return *this;
}

Vector2::Vector2(float x, float y) : x(x), y(y){}

Vector2::~Vector2(){}

const Vector2 Vector2::zero     = { 0.0F, 0.0F};
const Vector2 Vector2::one      = { 1.0F, 1.0F};
const Vector2 Vector2::up       = { 0.0F, 1.0F};
const Vector2 Vector2::down     = { 0.0F,-1.0F};
const Vector2 Vector2::left     = {-1.0F, 0.0F};
const Vector2 Vector2::right    = { 1.0F, 0.0F};

Vector2 Vector2::operator*(float w) const{
    return Vector2(x * w, y * w);
}

Vector2 &Vector2::operator*=(float w){
    x *= w; y *= w;
    return *this;
}

Vector2 Vector2::operator+(Vector2 v) const{
    return Vector2(x + v.x, y + v.y);
}

Vector2 &Vector2::operator+=(Vector2 v){
    x += v.x; y += v.y;
    return *this;
}

Vector2 Vector2::operator-() const{
    return Vector2(-x, -y);
}

Vector2 Vector2::operator-(Vector2 v) const{
    return Vector2(x - v.x, y - v.y);
}

Vector2 &Vector2::operator-=(Vector2 v){
    x -= v.x; y -= v.y;
    return *this;
}

Vector2 Vector2::operator/(float w) const{
    w = 1.0F / w;
    return Vector2(x * w, y * w);
}

Vector2 &Vector2::operator/=(float w){
    w = 1.0F / w;
    x *= w; y *= w;
    return *this;
}

float Vector2::Cross(Vector2 v1, Vector2 v2){
    return v1.x * v2.y - v1.y * v2.x;
}

float Vector2::Dot(Vector2 v1, Vector2 v2){
    return v1.x * v2.x + v1.y * v2.y;
}

float Vector2::Cosine(Vector2 v1, Vector2 v2){
    return Dot(v1, v2) / Sqrt(v1.SqrMagnitude() * v2.SqrMagnitude());
}

float Vector2::Sine(Vector2 v1, Vector2 v2){
    float val = Cosine(v1, v2);
    return Sqrt(1.0F - val * val);
}

float Vector2::Tangent(Vector2 v1, Vector2 v2){
    float val = Cosine(v1, v2);
    return Sqrt(1.0F - val * val) / val;
}

float Vector2::Magnitude() const{
    return Sqrt(x * x + y * y);
}

float Vector2::SqrMagnitude() const{
    return x * x + y * y;
}

Vector2 Vector2::Normal() const{
    return *this / this->Magnitude();
}

Vector2 &Vector2::Normalize(){
    return (*this /= this->Magnitude());
}

Vector2 Vector2::Reflect(Vector2 n) const{
    n *= (Dot(*this, n) / n.SqrMagnitude());
    return *this - n * 2.0F;
}

Vector2 Vector2::Refract(Vector2 n, float rate) const{
    n *= (Dot(*this, n) / n.SqrMagnitude());
    float val1 = Sine(*this, n);
    float val2 = val1 / rate;
    return n + (*this - n) * val2 / Sqrt(1.0F - val2 * val2) / val1 * Sqrt(1.0F - val1 * val1);
}

Vector2 Vector2::Rotate(float angle) const{
    float vsin, vcos;
    __builtin_sincosf(angle * 0.01745329251994329547f, &vsin, &vcos);
    return Vector2(x * vcos - y * vsin, x * vsin + y * vcos);
}

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f){}

Vector3::Vector3(Vector2 &&v) : x(v.x), y(v.y), z(0.0f){}

Vector3::Vector3(Vector4 &&v) : x(v.x), y(v.y), z(v.z){}

Vector3::Vector3(const Vector2 &v) : x(v.x), y(v.y), z(0.0f){}

Vector3::Vector3(const Vector4 &v) : x(v.x), y(v.y), z(v.z){}

Vector3 &Vector3::operator=(Vector2 &&v){
    this->x = v.x; this->y = v.y; this->z = 0.0f;
    return *this;
}

Vector3 &Vector3::operator=(Vector4 &&v){
    x = v.x; y = v.y; z = v.z;
    return *this;
}

Vector3 &Vector3::operator=(const Vector2 &v){
    this->x = v.x; this->y = v.y; this->z = 0.0f;
    return *this;
}

Vector3 &Vector3::operator=(const Vector4 &v){
    x = v.x; y = v.y; z = v.z;
    return *this;
}

Vector3::Vector3(float x, float y) : x(x), y(y), z(0.0f){}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z){}

Vector3::~Vector3(){}

const Vector3 Vector3::zero     = { 0.0F, 0.0F, 0.0F};
const Vector3 Vector3::one      = { 1.0F, 1.0F, 1.0F};
const Vector3 Vector3::up       = { 0.0F, 0.0F, 1.0F};
const Vector3 Vector3::down     = { 0.0F, 0.0F,-1.0F};
const Vector3 Vector3::left     = {-1.0F, 0.0F, 0.0F};
const Vector3 Vector3::right    = { 1.0F, 0.0F, 0.0F};
const Vector3 Vector3::forward  = { 0.0F, 1.0F, 0.0F};
const Vector3 Vector3::back     = { 0.0F,-1.0F, 0.0F};

Vector3 Vector3::operator*(float w) const{
    return Vector3(x * w, y * w, z * w);
}

Vector3 &Vector3::operator*=(float w){
    x *= w; y *= w; z *= w;
    return *this;
}

Vector3 Vector3::operator+(Vector3 v) const{
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 &Vector3::operator+=(Vector3 v){
    x += v.x; y += v.y; z += v.z;
    return *this;
}

Vector3 Vector3::operator-() const{
    return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator-(Vector3 v) const{
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 &Vector3::operator-=(Vector3 v){
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
}

Vector3 Vector3::operator/(float w) const{
    w = 1.0F / w;
    return Vector3(x * w, y * w, z * w);
}

Vector3 &Vector3::operator/=(float w){
    w = 1.0F / w;
    x *= w; y *= w; z *= w;
    return *this;
}

Vector3 &Vector3::operator*=(Quaternion q){
    return (*this = q * *this);
}

Vector3 &Vector3::operator/=(Quaternion q){
    return (*this = q.Inverse() * *this);
}

Vector3 Vector3::Cross(Vector3 v1, Vector3 v2){
    return Vector3( v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x);
}

float Vector3::Dot(Vector3 v1, Vector3 v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Vector3::Cosine(Vector3 v1, Vector3 v2){
    return Dot(v1, v2) / Sqrt(v1.SqrMagnitude() * v2.SqrMagnitude());
}

float Vector3::Sine(Vector3 v1, Vector3 v2){
    float val = Cosine(v1, v2);
    return Sqrt(1.0F - val * val);
}

float Vector3::Tangent(Vector3 v1, Vector3 v2){
    float val = Cosine(v1, v2);
    return Sqrt(1.0F - val * val) / val;
}

float Vector3::Determinant(Vector3 v1, Vector3 v2, Vector3 v3){
    return v1.x * (v2.y * v3.z - v2.z * v3.y) +
           v1.y * (v2.z * v3.x - v2.x * v3.z) +
           v1.z * (v2.x * v3.y - v2.y * v3.x);
}

Vector3 Vector3::Decompose(Vector3 vec, Vector3 x, Vector3 y, Vector3 z){
    float detinv = 1.0f / Determinant(x, y, z);
    return Vector3(Determinant(vec, y, z) * detinv,
                   Determinant(x, vec, z) * detinv,
                   Determinant(x, y, vec) * detinv);
}

float Vector3::Magnitude() const{
    return Sqrt(x * x + y * y + z * z);
}

float Vector3::SqrMagnitude() const{
    return x * x + y * y + z * z;
}

Vector3 Vector3::Normal() const{
    return *this / this->Magnitude();
}

Vector3 &Vector3::Normalize(){
    return (*this /= this->Magnitude());
}

Vector3 Vector3::Reflect(Vector3 n) const{
    n *= (Dot(*this, n) / n.SqrMagnitude());
    return *this - n * 2.0F;
}

Vector3 Vector3::Refract(Vector3 n, float rate) const{
    n *= (Dot(*this, n) / n.SqrMagnitude());
    float val1 = Sine(*this, n);
    float val2 = val1 / rate;
    return n + (*this - n) * val2 / Sqrt(1.0F - val2 * val2) / val1 * Sqrt(1.0F - val1 * val1);
}

Vector3 Vector3::RotateX(float angle) const{
    float vsin, vcos;
    __builtin_sincosf(angle * 0.01745329251994329547f, &vsin, &vcos);
    return Vector3(x, z * vsin + y * vcos, z * vcos - y * vsin);
}

Vector3 Vector3::RotateY(float angle) const{
    float vsin, vcos;
    __builtin_sincosf(angle * 0.01745329251994329547f, &vsin, &vcos);
    return Vector3(z * vsin + x * vcos, y, z * vcos - x * vsin);
}

Vector3 Vector3::RotateZ(float angle) const{
    float vsin, vcos;
    __builtin_sincosf(angle * 0.01745329251994329547f, &vsin, &vcos);
    return Vector3(x * vcos - y * vsin, x * vsin + y * vcos, z);
}

Vector4::Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f){}

Vector4::Vector4(Vector2 &&v) : x(v.x), y(v.y), z(0.0f), w(0.0f){}

Vector4::Vector4(Vector3 &&v) : x(v.x), y(v.y), z(v.z), w(0.0f){}

Vector4::Vector4(const Vector2 &v) : x(v.x), y(v.y), z(0.0f), w(0.0f){}

Vector4::Vector4(const Vector3 &v) : x(v.x), y(v.y), z(v.z), w(0.0f){}

Vector4 &Vector4::operator=(Vector2 &&v){
    this->x = v.x; this->y = v.y; this->z = 0.0f; this->w = 0.0f;
    return *this;
}

Vector4 &Vector4::operator=(Vector3 &&v){
    this->x = v.x; this->y = v.y; this->z = v.z; this->w = 0.0f;
    return *this;
}

Vector4 &Vector4::operator=(const Vector2 &v){
    this->x = v.x; this->y = v.y; this->z = 0.0f; this->w = 0.0f;
    return *this;
}

Vector4 &Vector4::operator=(const Vector3 &v){
    this->x = v.x; this->y = v.y; this->z = v.z; this->w = 0.0f;
    return *this;
}

Vector4::Vector4(float x, float y) : x(x), y(y), z(0.0f), w(0.0f){}

Vector4::Vector4(float x, float y, float z) : x(x), y(y), z(z), w(0.0f){}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){}

Vector4::Vector4(Vector3 v, float w) : x(v.x), y(v.y), z(v.z), w(w){}

Vector4::~Vector4(){}

const Vector4 Vector4::zero     = { 0.0F, 0.0F, 0.0F, 0.0F};
const Vector4 Vector4::one      = { 1.0F, 1.0F, 1.0F, 0.0F};
const Vector4 Vector4::up       = { 0.0F, 0.0F, 1.0F, 0.0F};
const Vector4 Vector4::down     = { 0.0F, 0.0F,-1.0F, 0.0F};
const Vector4 Vector4::left     = {-1.0F, 0.0F, 0.0F, 0.0F};
const Vector4 Vector4::right    = { 1.0F, 0.0F, 0.0F, 0.0F};
const Vector4 Vector4::forward  = { 0.0F, 1.0F, 0.0F, 0.0F};
const Vector4 Vector4::back     = { 0.0F,-1.0F, 0.0F, 0.0F};

Vector4 Vector4::operator+(Vector3 v) const{
    return Vector4(x + v.x, y + v.y, z + v.z, w);
}

Vector4 Vector4::operator+(Vector4 v) const{
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator-(Vector3 v) const{
    return Vector4(x - v.x, y - v.y, z - v.z, w);
}

Vector4 Vector4::operator-(Vector4 v) const{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator*(float w) const{
    return Vector4(x * w, y * w, z * w, this->w);
}

Vector4 Vector4::operator/(float w) const{
    w = 1.0F / w;
    return Vector4(x * w, y * w, z * w, this->w);
}

Vector4 &Vector4::operator+=(Vector3 v){
    x += v.x; y += v.y; z += v.z;
    return *this;
}

Vector4 &Vector4::operator-=(Vector3 v){
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
}

Vector4 &Vector4::operator*=(float w){
    x *= w; y *= w; z *= w;
    return *this;
}

Vector4 &Vector4::operator/=(float w){
    w = 1.0F / w;
    x *= w; y *= w; z *= w;
    return *this;
}

Vector4 Vector4::operator-() const{
    return Vector4(-x, -y, -z, w);
}

Vector4 &Vector4::operator*=(Quaternion q){
    return (*this = q * *this);
}

Vector4 &Vector4::operator/=(Quaternion q){
    return (*this = q.Inverse() * *this);
}

float Vector4::Dot(Vector4 v1, Vector4 v2){
    return Vector3::Dot(v1, v2);
}

Vector4 Vector4::Cross(Vector4 v1, Vector4 v2){
    return Vector3::Cross(v1, v2);
}

float Vector4::Cosine(Vector4 v1, Vector4 v2){
    return Vector3::Cosine(v1, v2);
}

float Vector4::Sine(Vector4 v1, Vector4 v2){
    return Vector3::Sine(v1, v2);
}

float Vector4::Tangent(Vector4 v1, Vector4 v2){
    return Vector3::Tangent(v1, v2);
}

Vector3 Vector4::Normal() const{
    return Vector3(x, y, z).Normal();
}

Vector4 &Vector4::Normalize(){
    return (*this = Vector3(x, y, z).Normal());
}

float Vector4::Magnitude() const{
    return Sqrt(x * x + y * y + z * z);
}

float Vector4::SqrMagnitude() const{
    return x * x + y * y + z * z;
}

Vector4 Vector4::Reflect(Vector3 n) const{
    n *= (Dot(*this, n) / n.SqrMagnitude());
    return *this - n * 2.0F;
}

Vector4 Vector4::Refract(Vector3 n, float rate) const{
    return Vector3(*this).Refract(n, rate);
}

Vector4 Vector4::RotateX(float angle) const{
    return Vector3(*this).RotateX(angle);
}

Vector4 Vector4::RotateY(float angle) const{
    return Vector3(*this).RotateY(angle);
}

Vector4 Vector4::RotateZ(float angle) const{
    return Vector3(*this).RotateZ(angle);
}

Quaternion::Quaternion(){}

Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Quaternion::Quaternion(Vector3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

Quaternion::~Quaternion(){}

const Quaternion Quaternion::one     = { 0.0F, 0.0F, 0.0F, 1.0F};

Quaternion Quaternion::operator*(Quaternion q) const{
    return Quaternion(  w * q.x + x * q.w + y * q.z - z * q.y,
                        w * q.y - x * q.z + y * q.w + z * q.x,
                        w * q.z + x * q.y - y * q.x + z * q.w,
                        w * q.w - x * q.x - y * q.y - z * q.z);//(w,z,-y,-x)*(-x,-y,-z,w)=(x2+w2-z2-y2,xy+wz-yw+yx,xz-wy+zx-yw,0)
}

Quaternion Quaternion::operator*(float s) const{
    return Quaternion(x * s, y * s, z * s, w * s);
}

Quaternion Quaternion::operator/(Quaternion q) const{
    return *this * q.Inverse();
}

Quaternion Quaternion::operator/(float s) const{
    s = 1.0F / s;
    return Quaternion(x * s, y * s, z * s, w * s);
}

Quaternion &Quaternion::operator*=(Quaternion q){
    return (*this = *this * q);
}

Quaternion &Quaternion::operator*=(float s){
    x *= s; y *= s; z *= s; w *= s;
    return *this;
}

Quaternion &Quaternion::operator/=(Quaternion q){
    return (*this *= *this * q.Inverse());
}

Quaternion &Quaternion::operator/=(float s){
    s = 1.0F / s;
    x *= s; y *= s; z *= s; w *= s;
    return *this;
}

Quaternion Quaternion::operator-() const{
    return Quaternion(-x, -y, -z, w);
}

Vector3 Quaternion::operator*(Vector3 v) const{
    return (*this * Quaternion(v, 0.0F) * this->Inverse()).GetAxis();
}

Quaternion Quaternion::FromTo(Vector3 from, Vector3 to){
    from.Normalize();
    to.Normalize();
    return Quaternion((from + to).Normal(), 0.0F) / Quaternion(from, 0.0F);
}

Quaternion Quaternion::Reflection(Vector3 axis, Vector3 v){
    return Quaternion(axis.Normal(), 0.0F) / Quaternion(v.Normal(), 0.0F);
}

Quaternion Quaternion::AxisAngle(Vector3 axis, float angle){
    angle *= 0.00872664625997164788461845384244F;
    return Quaternion(axis.Normal() * Sin(angle), Cos(angle));
}

Quaternion Quaternion::EulerZXY(Vector3 angle){
    return  Quaternion::AxisAngle(Vector3::up, angle.y) *
            Quaternion::AxisAngle(Vector3::right, angle.x) *
            Quaternion::AxisAngle(Vector3::forward, angle.z);
}

Quaternion Quaternion::EulerZXY(float x, float y, float z){
    return  Quaternion::AxisAngle(Vector3::up, y) *
            Quaternion::AxisAngle(Vector3::right, x) *
            Quaternion::AxisAngle(Vector3::forward, z);
}

Quaternion Quaternion::Conjugate() const{
    return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::Inverse() const{
    return Quaternion(-x, -y, -z, w) / this->SqrMagnitude();
}

Quaternion Quaternion::Normal() const{
    return *this / this->Magnitude();
}

Quaternion &Quaternion::Normalize(){
    return (*this /= this->Magnitude());
}

Quaternion &Quaternion::Rotate(Vector3 axis, float angle){
    return (*this *= AxisAngle(-*this * axis, angle) * *this);
}

float Quaternion::Magnitude() const{
    return Sqrt(x * x + y * y + z * z + w * w);
}

float Quaternion::SqrMagnitude() const{
    return x * x + y * y + z * z + w * w;
}

Vector3 Quaternion::GetAxis() const{
    return Vector3(x, y, z);
}

Matrix4x4::Matrix4x4(){}

Matrix4x4::Matrix4x4(Quaternion &&q){
    float x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
    float xy = q.x * q.y, yz = q.y * q.z, zw = q.z * q.w;
    float xz = q.x * q.z, yw = q.y * q.w, xw = q.x * q.w;
    _11 = 1.0f - 2.0f * (y2 + z2); _12 = 2.0f * (xy - zw); _13 = 2.0f * (xz - yw); _14 = 0.0f;
    _21 = 2.0f * (xy + zw); _22 = 1.0f - 2.0f * (z2 + x2); _23 = 2.0f * (yz - xw); _24 = 0.0f;
    _31 = 2.0f * (xz + yw); _32 = 2.0f * (yz + xw); _33 = 1.0f - 2.0f * (x2 + y2); _34 = 0.0f;
    _41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

Matrix4x4::Matrix4x4(const Quaternion &q){
    float x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
    float xy = q.x * q.y, yz = q.y * q.z, zw = q.z * q.w;
    float xz = q.x * q.z, yw = q.y * q.w, xw = q.x * q.w;
    _11 = 1.0f - 2.0f * (y2 + z2); _12 = 2.0f * (xy - zw); _13 = 2.0f * (xz - yw); _14 = 0.0f;
    _21 = 2.0f * (xy + zw); _22 = 1.0f - 2.0f * (z2 + x2); _23 = 2.0f * (yz - xw); _24 = 0.0f;
    _31 = 2.0f * (xz + yw); _32 = 2.0f * (yz + xw); _33 = 1.0f - 2.0f * (x2 + y2); _34 = 0.0f;
    _41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

Matrix4x4 &Matrix4x4::operator=(Quaternion &&q){
    float x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
    float xy = q.x * q.y, yz = q.y * q.z, zw = q.z * q.w;
    float xz = q.x * q.z, yw = q.y * q.w, xw = q.x * q.w;
    _11 = 1.0f - 2.0f * (y2 + z2); _12 = 2.0f * (xy - zw); _13 = 2.0f * (xz - yw); _14 = 0.0f;
    _21 = 2.0f * (xy + zw); _22 = 1.0f - 2.0f * (z2 + x2); _23 = 2.0f * (yz - xw); _24 = 0.0f;
    _31 = 2.0f * (xz + yw); _32 = 2.0f * (yz + xw); _33 = 1.0f - 2.0f * (x2 + y2); _34 = 0.0f;
    _41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
    return *this;
}

Matrix4x4 &Matrix4x4::operator=(const Quaternion &q){
    float x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z;
    float xy = q.x * q.y, yz = q.y * q.z, zw = q.z * q.w;
    float xz = q.x * q.z, yw = q.y * q.w, xw = q.x * q.w;
    _11 = 1.0f - 2.0f * (y2 + z2); _12 = 2.0f * (xy - zw); _13 = 2.0f * (xz - yw); _14 = 0.0f;
    _21 = 2.0f * (xy + zw); _22 = 1.0f - 2.0f * (z2 + x2); _23 = 2.0f * (yz - xw); _24 = 0.0f;
    _31 = 2.0f * (xz + yw); _32 = 2.0f * (yz + xw); _33 = 1.0f - 2.0f * (x2 + y2); _34 = 0.0f;
    _41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
    return *this;
}

Matrix4x4::Matrix4x4(float _11, float _12, float _13, float _14,
                     float _21, float _22, float _23, float _24,
                     float _31, float _32, float _33, float _34,
                     float _41, float _42, float _43, float _44) :
                     _11(_11), _12(_12), _13(_13), _14(_14),
                     _21(_21), _22(_22), _23(_23), _24(_24),
                     _31(_31), _32(_32), _33(_33), _34(_34),
                     _41(_41), _42(_42), _43(_43), _44(_44){}

Matrix4x4::~Matrix4x4(){}

const Matrix4x4 Matrix4x4::zero      = {0.0F, 0.0F, 0.0F, 0.0F,
                                        0.0F, 0.0F, 0.0F, 0.0F,
                                        0.0F, 0.0F, 0.0F, 0.0F,
                                        0.0F, 0.0F, 0.0F, 0.0F};
const Matrix4x4 Matrix4x4::identity  = {1.0F, 0.0F, 0.0F, 0.0F,
                                        0.0F, 1.0F, 0.0F, 0.0F,
                                        0.0F, 0.0F, 1.0F, 0.0F,
                                        0.0F, 0.0F, 0.0F, 1.0F};

Matrix4x4 Matrix4x4::operator+(Matrix4x4 m) const{
    return {_11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14,
            _21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24,
            _31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34,
            _41 + m._41, _42 + m._42, _43 + m._43, _44 + m._44};
}

Matrix4x4 Matrix4x4::operator-(Matrix4x4 m) const{
    return {_11 - m._11, _12 - m._12, _13 - m._13, _14 - m._14,
            _21 - m._21, _22 - m._22, _23 - m._23, _24 - m._24,
            _31 - m._31, _32 - m._32, _33 - m._33, _34 - m._34,
            _41 - m._41, _42 - m._42, _43 - m._43, _44 - m._44};
}

Matrix4x4 Matrix4x4::operator*(float w) const{
    return {_11 * w, _12 * w, _13 * w, _14 * w,
            _21 * w, _22 * w, _23 * w, _24 * w,
            _31 * w, _32 * w, _33 * w, _34 * w,
            _41 * w, _42 * w, _43 * w, _44 * w};
}

Matrix4x4 Matrix4x4::operator/(float w) const{
    w = 1.0F / w;
    return {_11 * w, _12 * w, _13 * w, _14 * w,
            _21 * w, _22 * w, _23 * w, _24 * w,
            _31 * w, _32 * w, _33 * w, _34 * w,
            _41 * w, _42 * w, _43 * w, _44 * w};
}

Matrix4x4 &Matrix4x4::operator+=(Matrix4x4 m){
    _11 += m._11; _12 += m._12; _13 += m._13; _14 += m._14;
    _21 += m._21; _22 += m._22; _23 += m._23; _24 += m._24;
    _31 += m._31; _32 += m._32; _33 += m._33; _34 += m._34;
    _41 += m._41; _42 += m._42; _43 += m._43; _44 += m._44;
    return *this;
}

Matrix4x4 &Matrix4x4::operator-=(Matrix4x4 m){
    _11 -= m._11; _12 -= m._12; _13 -= m._13; _14 -= m._14;
    _21 -= m._21; _22 -= m._22; _23 -= m._23; _24 -= m._24;
    _31 -= m._31; _32 -= m._32; _33 -= m._33; _34 -= m._34;
    _41 -= m._41; _42 -= m._42; _43 -= m._43; _44 -= m._44;
    return *this;
}

Matrix4x4 &Matrix4x4::operator*=(float w){
    _11 *= w; _12 *= w; _13 *= w; _14 *= w;
    _21 *= w; _22 *= w; _23 *= w; _24 *= w;
    _31 *= w; _32 *= w; _33 *= w; _34 *= w;
    _41 *= w; _42 *= w; _43 *= w; _44 *= w;
    return *this;
}

Matrix4x4 &Matrix4x4::operator/=(float w){
    w = 1.0F / w;
    _11 *= w; _12 *= w; _13 *= w; _14 *= w;
    _21 *= w; _22 *= w; _23 *= w; _24 *= w;
    _31 *= w; _32 *= w; _33 *= w; _34 *= w;
    _41 *= w; _42 *= w; _43 *= w; _44 *= w;
    return *this;
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4 m) const{
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

Matrix4x4 &Matrix4x4::operator*=(Matrix4x4 m){
    // 将矩阵视作变换，*=操作是加上变换的意思，对于矩阵来说应为左乘
    return (*this = m * *this);
}

Matrix4x4 Matrix4x4::operator-() const{
    return {-_11, -_12, -_13, -_14,
            -_21, -_22, -_23, -_24,
            -_31, -_32, -_33, -_34,
            -_41, -_42, -_43, -_44};
}

Matrix4x4 Matrix4x4::operator~() const{
    return {_11, _21, _31, _41,
            _12, _22, _32, _42,
            _13, _23, _33, _43,
            _14, _24, _34, _44};
}

Vector3 Matrix4x4::operator*(Vector3 v) const{
    return Vector3(
        _11 * v.x + _12 * v.y + _13 * v.z,
        _21 * v.x + _22 * v.y + _23 * v.z,
        _31 * v.x + _32 * v.y + _33 * v.z
    );
}

Vector4 Matrix4x4::operator*(Vector4 v) const{
    return Vector4(
        _11 * v.x + _12 * v.y + _13 * v.z + _14 * v.w,
        _21 * v.x + _22 * v.y + _23 * v.z + _24 * v.w,
        _31 * v.x + _32 * v.y + _33 * v.z + _34 * v.w,
        _41 * v.x + _42 * v.y + _43 * v.z + _44 * v.w
    );
}

float GetRate(float x, float a, float b){
    return (x - a) / (b - a);
}

float Lerp(float a, float b, float t){
    return (b - a) * t + a;
}

Vector2 Lerp(Vector2 a, Vector2 b, float t){
    return (b - a) * t + a;
}

Vector3 Lerp(Vector3 a, Vector3 b, float t){
    return (b - a) * t + a;
}

Quaternion Slerp(Quaternion a, Quaternion b, float t){
    Quaternion dif = b / a;
    float angle = Acos(dif.w);
    return Quaternion::AxisAngle(dif.GetAxis(), angle * t) * a;
}

int Clamp(int x, int a, int b){
    if (x <= a){
        return a;
    }
    if (x >= b){
        return b;
    }
    return x;
}

float Clamp(float x, float a, float b){
    if (x <= a){
        return a;
    }
    if (x >= b){
        return b;
    }
    return x;
}

void Sort(float& x, float& y){
    float t;
    if (x > y){
        t = x;
        x = y;
        y = t;
    }
}