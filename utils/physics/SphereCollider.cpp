#include <utils/physics/SphereCollider.h>

#include <utils/physics/AllColliders.h>

SphereCollider::SphereCollider(Rigidbody* rb) : ACollider(ColliderType::SPHERE) {
    this->rb = rb;
}

SphereCollider::~SphereCollider(){}

void SphereCollider::OnCollideCube(const CubeCollider* collider) const{}

void SphereCollider::OnCollideSphere(const SphereCollider* collider) const{}

void SphereCollider::OnCollideMesh(const MeshCollider* collider) const{}