#include <util/physics/Rigidbody.h>

#include <editor/main/ViewObject.h>
#include <util/math3d/LinearAlgebra.h>

Rigidbody::Rigidbody(AViewObject* object) : object(object) {}
Rigidbody::~Rigidbody(){}

Point3 Rigidbody::GetPosition() const{
    return object->transform.GetWorldTranslation();
}

Quaternion Rigidbody::GetRotation() const{
    return object->GetWorldRot();
}

AViewObject* Rigidbody::GetObject() const{
    return object;
}