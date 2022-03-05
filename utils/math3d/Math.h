#ifndef __UTILS_MATH3D_MATH__
#define __UTILS_MATH3D_MATH__

#include <define.h>

#define PI 3.1415926535897932384626433832795

#define Sqrt(x) (__builtin_sqrt(x))
#define Sin(x) (__builtin_sin(x))
#define Cos(x) (__builtin_cos(x))
#define Exp(x) (__builtin_exp(x))
#define Log(x) (__builtin_log(x))
#define Log2(x) (__builtin_log2(x))
#define Log10(x) (__builtin_log10(x))
#define Pow(x, y) (__builtin_pow(x, y))
#define Abs(x) (__builtin_fabs(x))
#define Maxf(x, y) (__builtin_fmax(x, y))
#define Minf(x, y) (__builtin_fmin(x, y))
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

template <typename T>
inline T Square(T x){ return x * x; }

float GetRate(float x, float a, float b);

float Lerp(float a, float b, float t);
Vector2 Lerp(Vector2 a, Vector2 b, float t);
Vector3 Lerp(Vector3 a, Vector3 b, float t);
Quaternion Slerp(Quaternion a, Quaternion b, float t);

int Clamp(int x, int a, int b);
float Clamp(float x, float a, float b);

// 排序为前小后大
void Sort(float& x, float& y);

template <typename T>
inline T Min(T a, T b){ return a < b ? a : b; }
template <typename T>
inline T Max(T a, T b){ return a > b ? a : b; }

#endif