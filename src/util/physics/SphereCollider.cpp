#include <util/physics/SphereCollider.h>

#include <util/os/Time.h>
#include <util/math3d/LinearAlgebra.h>
#include <util/physics/Rigidbody.h>
#include <util/physics/AllColliders.h>

SphereCollider::SphereCollider(Rigidbody* rb) : ACollider(ColliderType::SPHERE) {
    this->rb = rb;
}

SphereCollider::~SphereCollider(){}

void SphereCollider::OnCollideCube(const CubeCollider* collider) const{}

void SphereCollider::OnCollideSphere(const SphereCollider* collider) const{
    Vector3 pos = collider->GetPosition() - GetPosition();
    Rigidbody* rb2 = collider->GetRigidbody();
    Vector3 vel = Vector3::zero;
    float len;

    if (rb) vel -= rb->velocity;
    if (rb2) vel += rb2->velocity;

    len = radius + collider->radius;

    if (len * len <= (pos + vel * Time::GetDeltaTime()).SqrMagnitude()){
        Vector3 vel2 = vel.Reflect(pos);
        if (rb2)
            rb2->velocity = vel2 + (rb ? rb->velocity : Vector3::zero);
        if (rb)
            rb->velocity -= vel2 - vel;
    }
}

void SphereCollider::OnCollideMesh(const MeshCollider* collider) const{}