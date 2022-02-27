#include <utils/math3d/Math.h>

#include <utils/math3d/LinearAlgebra.h>

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