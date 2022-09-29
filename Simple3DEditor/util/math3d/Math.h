#ifndef __UTIL_MATH3D_MATH__
#define __UTIL_MATH3D_MATH__

#include <define.h>

#include <cmath>

const float PI = 3.14159265F;
const float PI_TIMES_2 = 6.28318530F;

inline float Sqrt(float x){ return std::sqrt(x); }
inline double Sqrt(double x){ return std::sqrt(x); }
inline long double Sqrt(long double x){ return std::sqrt(x); }
inline float Cbrt(float x){ return std::cbrt(x); }
inline double Cbrt(double x){ return std::cbrt(x); }
inline long double Cbrt(long double x){ return std::cbrt(x); }
inline float Sin(float x){ return std::sin(x); }
inline double Sin(double x){ return std::sin(x); }
inline long double Sin(long double x){ return std::sin(x); }
inline float Cos(float x){ return std::cos(x); }
inline double Cos(double x){ return std::cos(x); }
inline long double Cos(long double x){ return std::cos(x); }
inline float Exp(float x){ return std::exp(x); }
inline double Exp(double x){ return std::exp(x); }
inline long double Exp(long double x){ return std::exp(x); }
inline float Log(float x){ return std::log(x); }
inline double Log(double x){ return std::log(x); }
inline long double Log(long double x){ return std::log(x); }
inline float Log2(float x){ return std::log2(x); }
inline double Log2(double x){ return std::log2(x); }
inline long double Log2(long double x){ return std::log2(x); }
inline float Log10(float x){ return std::log10(x); }
inline double Log10(double x){ return std::log10(x); }
inline long double Log10(long double x){ return std::log10(x); }
inline float Pow(float x, float y){ return std::pow(x, y); }
inline double Pow(double x, double y){ return std::pow(x, y); }
inline long double Pow(long double x, long double y){ return std::pow(x, y); }
inline int Abs(int x){ return std::abs(x); }
inline float Abs(float x){ return std::fabs(x); }
inline double Abs(double x){ return std::fabs(x); }
inline long double Abs(long double x){ return std::fabs(x); }
inline float Mod(float x, float y){ return std::fmod(x, y); }
inline double Mod(double x, double y){ return std::fmod(x, y); }
inline long double Mod(long double x, long double y){ return std::fmod(x, y); }
inline float Max(float x, float y){ return std::fmax(x, y); }
inline double Max(double x, double y){ return std::fmax(x, y); }
inline long double Max(long double x, long double y){ return std::fmax(x, y); }
inline float Min(float x, float y){ return std::fmin(x, y); }
inline double Min(double x, double y){ return std::fmin(x, y); }
inline long double Min(long double x, long double y){ return std::fmin(x, y); }
inline float Floor(float x){ return std::floor(x); }
inline double Floor(double x){ return std::floor(x); }
inline long double Floor(long double x){ return std::floor(x); }
inline float Ceil(float x){ return std::ceil(x); }
inline double Ceil(double x){ return std::ceil(x); }
inline long double Ceil(long double x){ return std::ceil(x); }
inline float Round(float x){ return std::round(x); }
inline double Round(double x){ return std::round(x); }
inline long double Round(long double x){ return std::round(x); }
inline float Asin(float x){ return std::asin(x); }
inline double Asin(double x){ return std::asin(x); }
inline long double Asin(long double x){ return std::asin(x); }
inline float Acos(float x){ return std::acos(x); }
inline double Acos(double x){ return std::acos(x); }
inline long double Acos(long double x){ return std::acos(x); }
inline float Atan(float x){ return std::atan(x); }
inline double Atan(double x){ return std::atan(x); }
inline long double Atan(long double x){ return std::atan(x); }
// (1, 0)=>0 (0, 1)=>+90 (-1, 0)=>+180 (0, -1)=>-90 范围(-180, +180]
inline float Atan2(float x, float y){ return std::atan2(x, y); }
inline double Atan2(double x, double y){ return std::atan2(x, y); }
inline long double Atan2(long double x, long double y){ return std::atan2(x, y); }
inline float Sinh(float x){ return std::sinh(x); }
inline double Sinh(double x){ return std::sinh(x); }
inline long double Sinh(long double x){ return std::sinh(x); }
inline float Cosh(float x){ return std::cosh(x); }
inline double Cosh(double x){ return std::cosh(x); }
inline long double Cosh(long double x){ return std::cosh(x); }
inline float Tanh(float x){ return std::tanh(x); }
inline double Tanh(double x){ return std::tanh(x); }
inline long double Tanh(long double x){ return std::tanh(x); }
inline float Asinh(float x){ return std::asinh(x); }
inline double Asinh(double x){ return std::asinh(x); }
inline long double Asinh(long double x){ return std::asinh(x); }
inline float Acosh(float x){ return std::acosh(x); }
inline double Acosh(double x){ return std::acosh(x); }
inline long double Acosh(long double x){ return std::acosh(x); }
inline float Atanh(float x){ return std::atanh(x); }
inline double Atanh(double x){ return std::atanh(x); }
inline long double Atanh(long double x){ return std::atanh(x); }
inline float Copysign(float x, float sign){ return std::copysign(x, sign); }
inline double Copysign(double x, double sign){ return std::copysign(x, sign); }
inline long double Copysign(long double x, long double sign){ return std::copysign(x, sign); }
inline float InfinityF(){ return HUGE_VALF; }
inline double InfinityD(){ return HUGE_VAL; }
inline long double InfinityLD(){ return HUGE_VALL; }
inline float NanF(){ return std::nanf(""); }
inline double NanD(){ return std::nan(""); }
inline long double NanLD(){ return std::nanl(""); }
inline bool IsNaN(float x){ return std::isnan(x); }
inline bool IsNaN(double x){ return std::isnan(x); }
inline bool IsNaN(long double x){ return std::isnan(x); }
inline bool IsInf(float x){ return std::isinf(x); }
inline bool IsInf(double x){ return std::isinf(x); }
inline bool IsInf(long double x){ return std::isinf(x); }
inline bool IsFinite(float x){ return std::isfinite(x); }
inline bool IsFinite(double x){ return std::isfinite(x); }
inline bool IsFinite(long double x){ return std::isfinite(x); }
inline bool IsNormal(float x){ return std::isnormal(x); }
inline bool IsNormal(double x){ return std::isnormal(x); }
inline bool IsNormal(long double x){ return std::isnormal(x); }
// 正态分布曲线积分，2/Sqrt(PI)*积分(0-x){exp(-x^2)}
inline float Error(float x){  return std::erf(x); }
inline double Error(double x){  return std::erf(x); }
inline long double Error(long double x){  return std::erf(x); }

inline void SinCos(float x, float* vsin, float* vcos){ *vsin = std::sin(x); *vcos = std::cos(x); }
inline void SinCos(double x, double* vsin, double* vcos){ *vsin = std::sin(x); *vcos = std::cos(x); }
inline void SinCos(long double x, long double* vsin, long double* vcos){ *vsin = std::sin(x); *vcos = std::cos(x); }

inline float ToRadian(float x){ return x * 0.017453292519F; }
inline float ToRadianHalf(float x){ return x * 0.008726646259971F; }
inline float ToAngle(float x){ return x * 57.29577951308F; }

// 参数需要为定点数
template <typename T>
inline T Factorial(T x){ return (x == 0 ? 1 : x * Factorial(x - 1)); }

template <typename T>
inline T Square(T x){ return x * x; }

template <typename T>
inline T Saturate(T x){
    return (x <= 0 ? 0 : (x >= 1 ? 1 : x));
}

template <typename T>
inline T Clamp(T x, T a, T b){
    return (x <= a ? a : (x >= b ? b : x));
}

template <typename T>
inline T Lerp(T a, T b, float t){
    return (b - a) * t + a;
}

template <typename T>
inline T LerpClamped(T a, T b, float t){
    return (b - a) * Saturate(t) + a;
}

template <typename T>
inline T GetRate(T x, T a, T b){
    return (x - a) / (b - a);
}

template <typename T>
inline T GetRateClamped(T x, T a, T b){
    return Saturate((x - a) / (b - a));
}

// 排序为前小后大
template <typename T>
inline void Sort(T& x, T& y){
    if (x > y){
        T t;
        t = x; x = y; y = t;
    }
}

template <typename T>
inline T Min(T a, T b){ return a < b ? a : b; }
template <typename T>
inline T Max(T a, T b){ return a > b ? a : b; }

#endif