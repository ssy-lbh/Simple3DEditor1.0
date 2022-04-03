#ifndef __UTILS_MATH3D_CAMERA__
#define __UTILS_MATH3D_CAMERA__

#include <define.h>

#include <utils/math3d/LinearAlgebra.h>

class CCamera {
protected:
    void SetLookAt(Point3 at);
    void SetRotation(Quaternion rot);
    void SetDistance(float dis);

    Point3 GetLookPosition(Point3 pos);
    Point2 GetScreenPosition(Point3 pos);
    Point2 GetScreenPosition(Point3 pos, float aspect);

    void InitCamera(float aspect);
    void InitCamera(float aspect, float near, float far);

    CCamera();
    CCamera(Point3 pos, Point3 lookAt, Vector3 up);
    CCamera(Point3 pos, Point3 lookAt, Vector3 up, float dis);
    ~CCamera();

public:
    Point3 camLookat;
    Quaternion camDir = Quaternion::one;
    float camDis = 5.0f;
    Point3 camPos;
    Vector3 camRight = Vector3::right;
    Vector3 camUp = Vector3::up;
    Vector3 camForward = Vector3::forward;
};

#endif