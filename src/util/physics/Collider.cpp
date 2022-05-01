#include <util/physics/Collider.h>

#include <util/math3d/LinearAlgebra.h>
#include <util/physics/Rigidbody.h>
#include <util/physics/AllColliders.h>
#include <editor/main/ViewObject.h>

ACollider::ACollider(ColliderType type) : type(type) {}
ACollider::~ACollider(){}

ColliderType ACollider::GetType() const{
    return type;
}

Rigidbody* ACollider::GetRigidbody() const{
    return rb;
}

Point3 ACollider::GetPosition() const{
    return rb->GetPosition();
}

Quaternion ACollider::GetRotation() const{
    return rb->GetRotation();
}

void ACollider::OnCollide(const ACollider* collider) const{
    switch (collider->type){
    case ColliderType::CUBE:
        OnCollideCube(dynamic_cast<const CubeCollider*>(collider));
        break;
    case ColliderType::SPHERE:
        OnCollideSphere(dynamic_cast<const SphereCollider*>(collider));
        break;
    case ColliderType::MESH:
        OnCollideMesh(dynamic_cast<const MeshCollider*>(collider));
        break;
    }
}