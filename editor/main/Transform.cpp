#include <editor/main/Transform.h>

#include <utils/os/Log.h>
#include <utils/gl/GLUtils.h>

Transform::Transform() : position(Vector3::zero), rotation(Quaternion::one), rotationXYZ(Vector3::zero), scale(Vector3::one) {}
Transform::~Transform(){}

Vector3 Transform::GetWorldTranslation(){
    return chainMat.GetTranslation();
}

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

Vector3 Transform::GetRotationXYZ(){
    return rotationXYZ.Get();
}

Matrix4x4 Transform::GetMatrix(){
    Matrix4x4 mat = Matrix4x4::identity;

    mat._11 = scale.x.Get();
    mat._22 = scale.y.Get();
    mat._33 = scale.z.Get();

    if (rotationMode == ROT_QUATERNION){
        mat *= rotation.Get().Normal();
    }else{
        Quaternion rotX = Quaternion::RotateX(rotationXYZ.x.Get());
        Quaternion rotY = Quaternion::RotateY(rotationXYZ.y.Get());
        Quaternion rotZ = Quaternion::RotateZ(rotationXYZ.z.Get());

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

void Transform::SetRotationMode(RotationMode mode){
    rotationMode = mode;
    switch (rotationMode){
    case ROT_EULER_XYZ: rotationXYZ.Set(rotation.Get().ToEulerXYZ()); break;
    case ROT_EULER_XZY: rotationXYZ.Set(rotation.Get().ToEulerXZY()); break;
    case ROT_EULER_YXZ: rotationXYZ.Set(rotation.Get().ToEulerYXZ()); break;
    case ROT_EULER_YZX: rotationXYZ.Set(rotation.Get().ToEulerYZX()); break;
    case ROT_EULER_ZXY: rotationXYZ.Set(rotation.Get().ToEulerZXY()); break;
    case ROT_EULER_ZYX: rotationXYZ.Set(rotation.Get().ToEulerZYX()); break;
    }
}

void Transform::SetRotation(Quaternion rot){
    rotation.Set(rot);
    switch (rotationMode){
    case ROT_EULER_XYZ: rotationXYZ.Set(rotation.Get().ToEulerXYZ()); break;
    case ROT_EULER_XZY: rotationXYZ.Set(rotation.Get().ToEulerXZY()); break;
    case ROT_EULER_YXZ: rotationXYZ.Set(rotation.Get().ToEulerYXZ()); break;
    case ROT_EULER_YZX: rotationXYZ.Set(rotation.Get().ToEulerYZX()); break;
    case ROT_EULER_ZXY: rotationXYZ.Set(rotation.Get().ToEulerZXY()); break;
    case ROT_EULER_ZYX: rotationXYZ.Set(rotation.Get().ToEulerZYX()); break;
    }
}

void Transform::SetRotationXYZ(Vector3 xyz){
    rotationXYZ.Set(xyz);
    switch (rotationMode){
    case ROT_EULER_XYZ: rotation.Set(Quaternion::EulerXYZ(xyz)); break;
    case ROT_EULER_XZY: rotation.Set(Quaternion::EulerXZY(xyz)); break;
    case ROT_EULER_YXZ: rotation.Set(Quaternion::EulerYXZ(xyz)); break;
    case ROT_EULER_YZX: rotation.Set(Quaternion::EulerYZX(xyz)); break;
    case ROT_EULER_ZXY: rotation.Set(Quaternion::EulerZXY(xyz)); break;
    case ROT_EULER_ZYX: rotation.Set(Quaternion::EulerZYX(xyz)); break;
    }
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