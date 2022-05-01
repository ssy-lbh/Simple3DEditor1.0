#ifndef __UTILS_PHYSICS_CUBECOLLIDER__
#define __UTILS_PHYSICS_CUBECOLLIDER__

#include <define.h>

#include <util/math3d/LinearAlgebra.h>
#include <util/physics/Collider.h>

class CubeCollider final : public ACollider {
public:
    Vector3 scale = Vector3::one;

    CubeCollider(Rigidbody* rb);
    virtual ~CubeCollider() override;

    virtual void OnCollideCube(const CubeCollider* collider) const override;
    virtual void OnCollideSphere(const SphereCollider* collider) const override;
    virtual void OnCollideMesh(const MeshCollider* collider) const override;
};

#endif