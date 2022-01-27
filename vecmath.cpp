#include "vecmath.h"

Vector2::Vector2(){}

Vector2::~Vector2(){}

Vector2::Vector2(float x, float y) : x(x), y(y){}

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

Vector3::Vector3(){}

Vector3::~Vector3(){}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z){}

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

Quaternion::Quaternion(){}

Quaternion::~Quaternion(){}

Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Quaternion::Quaternion(Vector3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

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

float Quaternion::Magnitude() const{
    return Sqrt(x * x + y * y + z * z + w * w);
}

float Quaternion::SqrMagnitude() const{
    return x * x + y * y + z * z + w * w;
}

Vector3 Quaternion::GetAxis() const{
    return Vector3(x, y, z);
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