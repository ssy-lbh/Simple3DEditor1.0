#ifndef __UTILS_PHYSICS_MESHCOLLIDER__
#define __UTILS_PHYSICS_MESHCOLLIDER__

#include <define.h>

#include <utils/physics/Collider.h>

class MeshCollider final : public ACollider {
public:
    Mesh* mesh = NULL;

    MeshCollider(Rigidbody* rb);
    virtual ~MeshCollider();

    virtual void OnCollideCube(const CubeCollider* collider) const override;
    virtual void OnCollideSphere(const SphereCollider* collider) const override;
    virtual void OnCollideMesh(const MeshCollider* collider) const override;
};

#endif