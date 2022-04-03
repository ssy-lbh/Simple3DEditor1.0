#include <utils/physics/Rigidbody.h>

#include <editor/main/ViewObject.h>

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