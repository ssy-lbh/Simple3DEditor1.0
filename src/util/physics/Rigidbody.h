#ifndef __UTIL_PHYSICS_RIGIDBODY__
#define __UTIL_PHYSICS_RIGIDBODY__

#include <define.h>

#include <util/math3d/LinearAlgebra.h>

namespace simple3deditor {

// 物理学运算必要的数据就往里面放
class Rigidbody final : public Object {
private:
    // 运行时不能为NULL
    AViewObject* object = NULL;

public:
    Vector3 velocity = Vector3::zero;
    Vector3 torque = Vector3::zero;
    float mass = 1.0f;
    // 后面交给Mesh类计算吧，目前应与mass同步
    float angularInertia = 1.0f;
    bool useGravity = true;

    Rigidbody(AViewObject* object);
    ~Rigidbody();

    Point3 GetPosition() const;
    Quaternion GetRotation() const;
    AViewObject* GetObject() const;
};

}

#endif