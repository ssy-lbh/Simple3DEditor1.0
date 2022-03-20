#include <editor/main/Transform.h>

#include <utils/os/Log.h>
#include <utils/gl/GLUtils.h>

Transform::Transform() : position(Vector3::zero), rotation(Quaternion::one), rotationXYZ(Vector3::zero), scale(Vector3::one) {}
Transform::~Transform(){}

Quaternion Transform::GetRotation(){
    if (rotationMode == ROT_QUATERNION)
        return rotation.Get().Normal();

    Quaternion rotX = Quaternion::AxisAngle(Vector3::right, rotationXYZ.x.Get());
    Quaternion rotY = Quaternion::AxisAngle(Vector3::forward, rotationXYZ.y.Get());
    Quaternion rotZ = Quaternion::AxisAngle(Vector3::up, rotationXYZ.z.Get());

    switch (rotationMode){
    case ROT_EULER_XYZ: return rotZ * rotY * rotX;
    case ROT_EULER_XZY: return rotY * rotZ * rotX;
    case ROT_EULER_YXZ: return rotZ * rotX * rotY;
    case ROT_EULER_YZX: return rotX * rotZ * rotY;
    case ROT_EULER_ZXY: return rotY * rotX * rotZ;
    case ROT_EULER_ZYX: return rotX * rotY * rotZ;
    }

    DebugError("Transform::GetRotation Unknown Rotation Mode %d", rotationMode);

    return Quaternion::one;
}

Matrix4x4 Transform::GetMatrix(){
    Matrix4x4 mat = Matrix4x4::identity;

    mat._11 = scale.x.Get();
    mat._22 = scale.y.Get();
    mat._33 = scale.z.Get();

    if (rotationMode == ROT_QUATERNION){
        mat *= rotation.Get().Normal();
    }else{
        Quaternion rotX = Quaternion::AxisAngle(Vector3::right, rotationXYZ.x.Get());
        Quaternion rotY = Quaternion::AxisAngle(Vector3::forward, rotationXYZ.y.Get());
        Quaternion rotZ = Quaternion::AxisAngle(Vector3::up, rotationXYZ.z.Get());

        switch (rotationMode){
        case ROT_EULER_XYZ:
            mat *= (rotZ * rotY * rotX);
            break;
        case ROT_EULER_XZY:
            mat *= (rotY * rotZ * rotX);
            break;
        case ROT_EULER_YXZ:
            mat *= (rotZ * rotX * rotY);
            break;
        case ROT_EULER_YZX:
            mat *= (rotX * rotZ * rotY);
            break;
        case ROT_EULER_ZXY:
            mat *= (rotY * rotX * rotZ);
            break;
        case ROT_EULER_ZYX:
            mat *= (rotX * rotY * rotZ);
            break;
        }
    }

    mat._14 = position.x.Get();
    mat._24 = position.y.Get();
    mat._34 = position.z.Get();

    // 调试输出矩阵
    // DebugLog("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f",
    //          mat._11, mat._12, mat._13, mat._14,
    //          mat._21, mat._22, mat._23, mat._24,
    //          mat._31, mat._32, mat._33, mat._34,
    //          mat._41, mat._42, mat._43, mat._44);

    return mat;
}

Matrix4x4 Transform::GetInvMatrix(){
    Matrix4x4 mat = Matrix4x4::identity;

    mat._14 = -position.x.Get();
    mat._24 = -position.y.Get();
    mat._34 = -position.z.Get();

    if (rotationMode == ROT_QUATERNION){
        mat *= -rotation.Get().Normal();
    }else{
        Quaternion rotX = Quaternion::AxisAngle(Vector3::right, rotationXYZ.x.Get());
        Quaternion rotY = Quaternion::AxisAngle(Vector3::forward, rotationXYZ.y.Get());
        Quaternion rotZ = Quaternion::AxisAngle(Vector3::up, rotationXYZ.z.Get());

        switch (rotationMode){
        case ROT_EULER_XYZ:
            mat *= -(rotZ * rotY * rotX);
            break;
        case ROT_EULER_XZY:
            mat *= -(rotY * rotZ * rotX);
            break;
        case ROT_EULER_YXZ:
            mat *= -(rotZ * rotX * rotY);
            break;
        case ROT_EULER_YZX:
            mat *= -(rotX * rotZ * rotY);
            break;
        case ROT_EULER_ZXY:
            mat *= -(rotY * rotX * rotZ);
            break;
        case ROT_EULER_ZYX:
            mat *= -(rotX * rotY * rotZ);
            break;
        }
    }

    float tmp;

    tmp = 1.0f / scale.x.Get(); mat._11 *= tmp; mat._12 *= tmp; mat._13 *= tmp;
    tmp = 1.0f / scale.y.Get(); mat._21 *= tmp; mat._22 *= tmp; mat._23 *= tmp;
    tmp = 1.0f / scale.z.Get(); mat._31 *= tmp; mat._32 *= tmp; mat._33 *= tmp;

    return mat;
}

void Transform::PushMatrix(){
    GLUtils::PushMatrix(GetMatrix());
}

void Transform::PushInvMatrix(){
    GLUtils::PushMatrix(GetInvMatrix());
}

void Transform::PopMatrix(){
    GLUtils::PopMatrix();
}

void Transform::SetFrame(float frame){
    position.SetFrame(frame);
    rotation.SetFrame(frame);
    rotationXYZ.SetFrame(frame);
    scale.SetFrame(frame);
}

void Transform::InsertPos(float frame){
    position.InsertValue(frame);
}

void Transform::InsertRot(float frame){
    if (rotationMode == ROT_QUATERNION){
        rotation.InsertValue(frame);
    }else{
        rotationXYZ.InsertValue(frame);
    }
}

void Transform::InsertScale(float frame){
    scale.InsertValue(frame);
}