#ifndef __UTILS_PHYSICS_SPHERECOLLIDER__
#define __UTILS_PHYSICS_SPHERECOLLIDER__

#include <define.h>

#include <utils/physics/Collider.h>

class SphereCollider final : public ACollider {
public:
    SphereCollider(Rigidbody* rb);
    virtual ~SphereCollider();

    virtual void OnCollideCube(const CubeCollider* collider) const override;
    virtual void OnCollideSphere(const SphereCollider* collider) const override;
    virtual void OnCollideMesh(const MeshCollider* collider) const override;
};

#endif