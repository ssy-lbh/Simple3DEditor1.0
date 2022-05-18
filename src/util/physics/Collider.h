#ifndef __UTIL_PHYSICS_COLLIDER__
#define __UTIL_PHYSICS_COLLIDER__

#include <define.h>

namespace simple3deditor {

enum class ColliderType {
    CUBE,
    SPHERE,
    MESH
};

class ACollider : public Object {
protected:
    const ColliderType type;

    // 运行时若为NULL则速度等物理效果无法产生
    Rigidbody* rb = NULL;

    ACollider(ColliderType type);

public:
    virtual ~ACollider();

    ColliderType GetType() const;
    Rigidbody* GetRigidbody() const;
    Point3 GetPosition() const;
    Quaternion GetRotation() const;

    void OnCollide(const ACollider* collider) const;

    virtual void OnCollideCube(const CubeCollider* collider) const = 0;
    virtual void OnCollideSphere(const SphereCollider* collider) const = 0;
    virtual void OnCollideMesh(const MeshCollider* collider) const = 0;
};

}

#endif