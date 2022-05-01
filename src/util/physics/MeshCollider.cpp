#include <util/physics/MeshCollider.h>

#include <main.h>
#include <util/physics/Rigidbody.h>
#include <util/physics/AllColliders.h>

MeshCollider::MeshCollider(Rigidbody* rb) : ACollider(ColliderType::MESH) {
    this->rb = rb;
    this->mesh = Main::GetMesh(rb->GetObject());
}

MeshCollider::~MeshCollider(){}

void MeshCollider::OnCollideCube(const CubeCollider* collider) const{}

void MeshCollider::OnCollideSphere(const SphereCollider* collider) const{}

void MeshCollider::OnCollideMesh(const MeshCollider* collider) const{}