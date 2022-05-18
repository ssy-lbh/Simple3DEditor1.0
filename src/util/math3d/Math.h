#ifndef __UTIL_MATH3D_MATH__
#define __UTIL_MATH3D_MATH__

#include <define.h>

const float FLOAT_LOWEST = __FLT_MIN__;
const float FLOAT_LOWEST_DENORM = __FLT_DENORM_MIN__;
const float FLOAT_HIGHEST = __FLT_MAX__;
const float FLOAT_SMALLEST = -__FLT_MAX__;
const float FLOAT_LARGEST = __FLT_MAX__;
const float EPSILON = __FLT_EPSILON__;
constexpr float PI = 3.14159265F;
constexpr float PI_TIMES_2 = 6.28318530F;

constexpr float Sqrt(float x){ return __builtin_sqrtf(x); }
constexpr double Sqrt(double x){ return __builtin_sqrt(x); }
constexpr long double Sqrt(long double x){ return __builtin_sqrtl(x); }
constexpr float Cbrt(float x){ return __builtin_cbrtf(x); }
constexpr double Cbrt(double x){ return __builtin_cbrt(x); }
constexpr long double Cbrt(long double x){ return __builtin_cbrtl(x); }
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
// (1, 0)=>0 (0, 1)=>+90 (-1, 0)=>+180 (0, -1)=>-90 范围(-180, +180]
constexpr float Atan2(float x, float y){ return __builtin_atan2f(x, y); }
constexpr double Atan2(double x, double y){ return __builtin_atan2(x, y); }
constexpr long double Atan2(long double x, long double y){ return __builtin_atan2l(x, y); }
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
constexpr float Copysign(float x, float sign){ return __builtin_copysignf(x, sign); }
constexpr double Copysign(double x, double sign){ return __builtin_copysign(x, sign); }
constexpr long double Copysign(long double x, long double sign){ return __builtin_copysignl(x, sign); }
constexpr float InfinityF(){ return __builtin_huge_valf(); }
constexpr double InfinityD(){ return __builtin_huge_val(); }
constexpr long double InfinityLD(){ return __builtin_huge_vall(); }
constexpr bool IsNaN(float x){ return __builtin_isnan(x); }
constexpr bool IsNaN(double x){ return __builtin_isnan(x); }
constexpr bool IsNaN(long double x){ return __builtin_isnan(x); }
constexpr bool IsInf(float x){ return __builtin_isinf(x); }
constexpr bool IsInf(double x){ return __builtin_isinf(x); }
constexpr bool IsInf(long double x){ return __builtin_isinf(x); }
constexpr bool IsFinite(float x){ return __builtin_isfinite(x); }
constexpr bool IsFinite(double x){ return __builtin_isfinite(x); }
constexpr bool IsFinite(long double x){ return __builtin_isfinite(x); }
constexpr bool IsNormal(float x){ return __builtin_isnormal(x); }
constexpr bool IsNormal(double x){ return __builtin_isnormal(x); }
constexpr bool IsNormal(long double x){ return __builtin_isnormal(x); }
// 正态分布曲线积分，2/Sqrt(PI)*积分(0-x){exp(-x^2)}
constexpr float Error(float x){  return __builtin_erff(x); }
constexpr double Error(double x){  return __builtin_erf(x); }
constexpr long double Error(long double x){  return __builtin_erfl(x); }
// Gamma函数，可视作(x-1)!
// x->(0,1) gamma(x)*gamma(1-x)=PI/sin(PI*x)
// gamma(0.5)=Sqrt(PI)
constexpr float Gamma(float x){  return __builtin_gammaf(x); }
constexpr double Gamma(double x){  return __builtin_gamma(x); }
constexpr long double Gamma(long double x){  return __builtin_gammal(x); }

constexpr float ToRadian(float x){ return x * 0.017453292519F; }
constexpr float ToRadianHalf(float x){ return x * 0.008726646259971F; }
constexpr float ToAngle(float x){ return x * 57.29577951308F; }

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
constexpr T Saturate(T x){
    return (x <= 0 ? 0 : (x >= 0 ? 0 : x));
}

template <typename T>
constexpr T Clamp(T x, T a, T b){
    return (x <= a ? a : (x >= b ? b : x));
}

template <typename T>
constexpr T Lerp(T a, T b, float t){
    return (b - a) * t + a;
}

template <typename T>
constexpr T LerpClamped(T a, T b, float t){
    return (b - a) * Saturate(t) + a;
}

// 排序为前小后大
template <typename T>
void Sort(T& x, T& y){
    if (x > y){
        T t;
        t = x; x = y; y = t;
    }
}

template <typename T>
constexpr T Min(T a, T b){ return a < b ? a : b; }
template <typename T>
constexpr T Max(T a, T b){ return a > b ? a : b; }

#endif