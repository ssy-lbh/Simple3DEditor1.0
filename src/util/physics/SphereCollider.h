#ifndef __UTIL_PHYSICS_SPHERECOLLIDER__
#define __UTIL_PHYSICS_SPHERECOLLIDER__

#include <define.h>

#include <util/physics/Collider.h>

namespace simple3deditor {

class SphereCollider final : public ACollider {
public:
    float radius = 1.0f;

    SphereCollider(Rigidbody* rb);
    virtual ~SphereCollider();

    virtual void OnCollideCube(const CubeCollider* collider) const override;
    virtual void OnCollideSphere(const SphereCollider* collider) const override;
    virtual void OnCollideMesh(const MeshCollider* collider) const override;
};

}

#endif