#ifndef __UTILS_MATH3D_MATH__
#define __UTILS_MATH3D_MATH__

#include <define.h>

#define PI 3.1415926535897932384626433832795

constexpr float Sqrt(float x){ return __builtin_sqrtf(x); }
constexpr double Sqrt(double x){ return __builtin_sqrt(x); }
constexpr long double Sqrt(long double x){ return __builtin_sqrtl(x); }
constexpr float Sin(float x){ return __builtin_sinf(x); }
constexpr double Sin(double x){ return __builtin_sin(x); }
constexpr long double Sin(long double x){ return __builtin_sinl(x); }
constexpr float Cos(float x){ return __builtin_cosf(x); }
constexpr double Cos(double x){ return __builtin_cos(x); }
constexpr long double Cos(long double x){ return __builtin_cosl(x); }
constexpr float Exp(float x){ return __builtin_expf(x); }
constexpr double Exp(double x){ return __builtin_exp(x); }
constexpr long double Exp(long double x){ return __builtin_expl(x); }
constexpr float Log(float x){ return __builtin_logf(x); }
constexpr double Log(double x){ return __builtin_log(x); }
constexpr long double Log(long double x){ return __builtin_logl(x); }
constexpr float Log2(float x){ return __builtin_log2f(x); }
constexpr double Log2(double x){ return __builtin_log2(x); }
constexpr long double Log2(long double x){ return __builtin_log2l(x); }
constexpr float Log10(float x){ return __builtin_log10f(x); }
constexpr double Log10(double x){ return __builtin_log10(x); }
constexpr long double Log10(long double x){ return __builtin_log10l(x); }
constexpr float Pow(float x, float y){ return __builtin_powf(x, y); }
constexpr double Pow(double x, double y){ return __builtin_pow(x, y); }
constexpr long double Pow(long double x, long double y){ return __builtin_powl(x, y); }
constexpr int Abs(int x){ return __builtin_abs(x); }
constexpr float Abs(float x){ return __builtin_fabsf(x); }
constexpr double Abs(double x){ return __builtin_fabs(x); }
constexpr long double Abs(long double x){ return __builtin_fabsl(x); }
constexpr float Mod(float x, float y){ return __builtin_fmodf(x, y); }
constexpr double Mod(double x, double y){ return __builtin_fmod(x, y); }
constexpr long double Mod(long double x, long double y){ return __builtin_fmodl(x, y); }
constexpr float Max(float x, float y){ return __builtin_fmaxf(x, y); }
constexpr double Max(double x, double y){ return __builtin_fmax(x, y); }
constexpr long double Max(long double x, long double y){ return __builtin_fmaxl(x, y); }
constexpr float Min(float x, float y){ return __builtin_fminf(x, y); }
constexpr double Min(double x, double y){ return __builtin_fmin(x, y); }
constexpr long double Min(long double x, long double y){ return __builtin_fminl(x, y); }
constexpr float Floor(float x){ return __builtin_floorf(x); }
constexpr double Floor(double x){ return __builtin_floor(x); }
constexpr long double Floor(long double x){ return __builtin_floorl(x); }
constexpr float Ceil(float x){ return __builtin_ceilf(x); }
constexpr double Ceil(double x){ return __builtin_ceil(x); }
constexpr long double Ceil(long double x){ return __builtin_ceill(x); }
constexpr float Round(float x){ return __builtin_roundf(x); }
constexpr double Round(double x){ return __builtin_round(x); }
constexpr long double Round(long double x){ return __builtin_roundl(x); }
inline void SinCos(float x, float* vsin, float* vcos){ return __builtin_sincosf(x, vsin, vcos); }
inline void SinCos(double x, double* vsin, double* vcos){ return __builtin_sincos(x, vsin, vcos); }
inline void SinCos(long double x, long double* vsin, long double* vcos){ return __builtin_sincosl(x, vsin, vcos); }
constexpr float Asin(float x){ return __builtin_asinf(x); }
constexpr double Asin(double x){ return __builtin_asin(x); }
constexpr long double Asin(long double x){ return __builtin_asinl(x); }
constexpr float Acos(float x){ return __builtin_acosf(x); }
constexpr double Acos(double x){ return __builtin_acos(x); }
constexpr long double Acos(long double x){ return __builtin_acosl(x); }
constexpr float Atan(float x){ return __builtin_atanf(x); }
constexpr double Atan(double x){ return __builtin_atan(x); }
constexpr long double Atan(long double x){ return __builtin_atanl(x); }
constexpr float Sinh(float x){ return __builtin_sinhf(x); }
constexpr double Sinh(double x){ return __builtin_sinh(x); }
constexpr long double Sinh(long double x){ return __builtin_sinhl(x); }
constexpr float Cosh(float x){ return __builtin_coshf(x); }
constexpr double Cosh(double x){ return __builtin_cosh(x); }
constexpr long double Cosh(long double x){ return __builtin_coshl(x); }
constexpr float Tanh(float x){ return __builtin_tanhf(x); }
constexpr double Tanh(double x){ return __builtin_tanh(x); }
constexpr long double Tanh(long double x){ return __builtin_tanhl(x); }
constexpr float Asinh(float x){ return __builtin_asinhf(x); }
constexpr double Asinh(double x){ return __builtin_asinh(x); }
constexpr long double Asinh(long double x){ return __builtin_asinhl(x); }
constexpr float Acosh(float x){ return __builtin_acoshf(x); }
constexpr double Acosh(double x){ return __builtin_acosh(x); }
constexpr long double Acosh(long double x){ return __builtin_acoshl(x); }
constexpr float Atanh(float x){ return __builtin_atanhf(x); }
constexpr double Atanh(double x){ return __builtin_atanh(x); }
constexpr long double Atanh(long double x){ return __builtin_atanhl(x); }

constexpr float ToRadian(float x){ return x * 0.017453292519f; }
constexpr float ToAngle(float x){ return x * 57.29577951308f; }

// 参数需要为定点数
template <typename T>
constexpr T Factorial(T x){ return (x == 0 ? 1 : x * Factorial(x - 1)); }

template <typename T>
constexpr T Square(T x){ return x * x; }

template <typename T>
constexpr T GetRate(T x, T a, T b){
    return (x - a) / (b - a);
}

template <typename T>
constexpr T Lerp(T a, T b, float t){
    return (b - a) * t + a;
}

Quaternion Slerp(Quaternion a, Quaternion b, float t);

template <typename T>
constexpr T Clamp(T x, T a, T b){
    return (x <= a ? a : (x >= b ? b : x));
}

// 排序为前小后大
inline void Sort(float& x, float& y){
    float t;
    if (x > y){
        t = x; x = y; y = t;
    }
}

template <typename T>
constexpr T Min(T a, T b){ return a < b ? a : b; }
template <typename T>
constexpr T Max(T a, T b){ return a > b ? a : b; }

#endif