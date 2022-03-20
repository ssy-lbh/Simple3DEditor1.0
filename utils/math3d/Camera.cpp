#include <utils/math3d/Camera.h>

#include <lib/opengl/gl/gl.h>
#include <lib/opengl/gl/glu.h>

Camera::Camera() : camPos(Point3::zero), camLookat(Point3(0.0f, 5.0f, 0.0f)), camDis(5.0f) {
    SetRotation(Quaternion::one);
}

Camera::Camera(Point3 pos, Point3 lookAt, Vector3 up) : camPos(pos), camLookat(lookAt), camDis(5.0f) {
    SetRotation(Quaternion::LookAt(lookAt - pos, up));
}

Camera::Camera(Point3 pos, Point3 lookAt, Vector3 up, float dis) : camPos(pos), camLookat(lookAt), camDis(dis) {
    SetRotation(Quaternion::LookAt(lookAt - pos, up));
}

Camera::~Camera(){}

void Camera::SetLookAt(Point3 at){
    camLookat = at;
    camPos = camLookat - camForward * camDis;
}

void Camera::SetRotation(Quaternion rot){
    camDir = rot;
    camForward = camDir * Vector3::forward;
    camUp = camDir * Vector3::up;
    camRight = camDir * Vector3::right;
    camPos = camLookat - camForward * camDis;
}

void Camera::SetDistance(float dis){
    camDis = dis;
    camPos = camLookat - camForward * camDis;
}

Point3 Camera::GetLookPosition(Point3 pos){
    return (-camDir) * (pos - camPos);
}

Point2 Camera::GetScreenPosition(Point3 pos){
    Vector3 lookPos = (-camDir) * (pos - camPos);
    float invY = 1.0f / lookPos.y;
    return Point2(lookPos.x * invY, lookPos.z * invY);
}

Point2 Camera::GetScreenPosition(Point3 pos, float aspect){
    Vector3 lookPos = (-camDir) * (pos - camPos);
    float invY = 1.0f / lookPos.y;
    return Point2((lookPos.x * invY) / aspect, lookPos.z * invY);
}

void Camera::InitCamera(float aspect){
    InitCamera(aspect, camDis * 0.02f, camDis * 20.0f);
}

void Camera::InitCamera(float aspect, float near, float far){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, aspect, near, far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPos.x, camPos.y, camPos.z,
            camLookat.x, camLookat.y, camLookat.z,
            camUp.x, camUp.y, camUp.z);
}