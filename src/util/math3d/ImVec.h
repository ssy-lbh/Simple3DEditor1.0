#ifndef __UTIL_MATH3D_IMVEC__
#define __UTIL_MATH3D_IMVEC__

#include <define.h>

#include <util/math3d/Math.h>

#include <lib/imgui/imgui.h>

namespace simple3deditor {

// 常引用对左值与右值都可以
inline ImVec2 operator*(const ImVec2& v, float w){
    return ImVec2(v.x * w, v.y * w);
}

inline ImVec2 operator*(float w, const ImVec2& v){
    return ImVec2(v.x * w, v.y * w);
}

inline ImVec2 operator*(const ImVec2& v1, const ImVec2& v2){
    return ImVec2(v1.x * v2.x, v1.y * v2.y);
}

inline ImVec2& operator*=(ImVec2& v, float w){
    v.x *= w; v.y *= w;
    return v;
}

inline ImVec2 operator+(const ImVec2& v1, const ImVec2& v2){
    return ImVec2(v1.x + v2.x, v1.y + v2.y);
}

inline ImVec2& operator+=(ImVec2& v1, const ImVec2& v2){
    v1.x += v2.x; v1.y += v2.y;
    return v1;
}

inline ImVec2 operator-(const ImVec2& v){
    return ImVec2(-v.x, -v.y);
}

inline ImVec2 operator-(const ImVec2& v1, const ImVec2& v2){
    return ImVec2(v1.x - v2.x, v1.y - v2.y);
}

inline ImVec2& operator-=(ImVec2& v1, const ImVec2& v2){
    v1.x -= v2.x; v1.y -= v2.y;
    return v1;
}

inline ImVec2 operator/(const ImVec2& v, float w){
    w = 1.0F / w;
    return ImVec2(v.x * w, v.y * w);
}

inline ImVec2 operator/(const ImVec2& v1, const ImVec2& v2){
    return ImVec2(v1.x / v2.x, v1.y / v2.y);
}

inline ImVec2& operator/=(ImVec2& v, float w){
    w = 1.0F / w;
    v.x *= w; v.y *= w;
    return v;
}

inline float Cross(const ImVec2& v1, const ImVec2& v2){
    return v1.x * v2.y - v1.y * v2.x;
}

inline float Dot(const ImVec2& v1, const ImVec2& v2){
    return v1.x * v2.x + v1.y * v2.y;
}

inline float Magnitude(const ImVec2& v){
    return Sqrt(v.x * v.x + v.y * v.y);
}

inline float SqrMagnitude(const ImVec2& v){
    return v.x * v.x + v.y * v.y;
}

inline float Cosine(const ImVec2& v1, const ImVec2& v2){
    return Dot(v1, v2) / Sqrt(SqrMagnitude(v1) * SqrMagnitude(v2));
}

inline float Sine(const ImVec2& v1, const ImVec2& v2){
    float val = Cosine(v1, v2);
    return Sqrt(1.0F - val * val);
}

inline float Tangent(const ImVec2& v1, const ImVec2& v2){
    float val = Cosine(v1, v2);
    return Sqrt(1.0F - val * val) / val;
}

inline ImVec2 Normal(const ImVec2& v){
    return v / Magnitude(v);
}

inline ImVec2& Normalize(ImVec2& v){
    return (v /= Magnitude(v));
}

inline ImVec2 Reflect(const ImVec2& v, const ImVec2& n){
    return v - n * ((Dot(v, n) / SqrMagnitude(n)) * 2.0F);
}

inline ImVec2 Refract(const ImVec2& v, const ImVec2& n, float rate){
    ImVec2 n2 = n * (Dot(v, n) / SqrMagnitude(n));
    float val1 = Sine(v, n2);
    float val2 = val1 / rate;
    return n2 + (v - n2) * val2 / Sqrt(1.0F - val2 * val2) / val1 * Sqrt(1.0F - val1 * val1);
}

// Counter Clockwise
inline ImVec2 Rotate(const ImVec2& v, float angle){
    float vsin, vcos;
    SinCos(ToRadian(angle), &vsin, &vcos);
    return ImVec2(v.x * vcos - v.y * vsin, v.x * vsin + v.y * vcos);
}

}

#endif