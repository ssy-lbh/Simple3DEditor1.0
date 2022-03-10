#include <utils/math3d/Math.h>

#include <utils/math3d/LinearAlgebra.h>

Quaternion Slerp(Quaternion a, Quaternion b, float t){
    Quaternion dif = b / a;
    float angle = Acos(dif.w);
    return Quaternion::AxisAngle(dif.GetAxis(), angle * t) * a;
}