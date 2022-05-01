#include <util/physics/CubeCollider.h>

#include <util/os/Time.h>
#include <util/math3d/Math.h>
#include <util/math3d/LinearAlgebra.h>
#include <util/physics/Rigidbody.h>
#include <util/physics/AllColliders.h>

static bool CastToBox(const Vector3& pos, const Vector3& dir, const Vector3& scale, Vector3& output){
    if (pos.x > scale.x && dir.x < 0.0f){
        output = dir * ((scale.x - pos.x) / dir.x) + pos;
        if (Abs(output.y) <= scale.y && Abs(output.z) <= scale.z)
            return true;
    }else if (pos.x < -scale.x && dir.x > 0.0f){
        output = dir * ((-scale.x - pos.x) / dir.x) + pos;
        if (Abs(output.y) <= scale.y && Abs(output.z) <= scale.z)
            return true;
    }
    if (pos.y > scale.y && dir.y < 0.0f){
        output = dir * ((scale.y - pos.y) / dir.y) + pos;
        if (Abs(output.x) <= scale.x && Abs(output.z) <= scale.z)
            return true;
    }else if (pos.y < -scale.y && dir.y > 0.0f){
        output = dir * ((-scale.y - pos.y) / dir.y) + pos;
        if (Abs(output.x) <= scale.x && Abs(output.z) <= scale.z)
            return true;
    }
    if (pos.z > scale.z && dir.z < 0.0f){
        output = dir * ((scale.z - pos.z) / dir.z) + pos;
        if (Abs(output.x) <= scale.x && Abs(output.y) <= scale.y)
            return true;
    }else if (pos.z < -scale.z && dir.z > 0.0f){
        output = dir * ((-scale.z - pos.z) / dir.z) + pos;
        if (Abs(output.x) <= scale.x && Abs(output.y) <= scale.y)
            return true;
    }
    return false;
}

static bool CastLength2(const Vector3& pos, const Vector3& dir, const Vector3& scale, Vector3& res, float& len2){
    Vector3 vec;
    if (!CastToBox(pos, dir, scale, vec))
        return false;
    res = vec;
    len2 = (vec - pos).SqrMagnitude();
    return true;
}

static bool CastMinLength(const Vector3& pos, const Vector3& dir, const Vector3& scale, const Vector3& scale2, const Quaternion& rot, Vector3& result, float& minlen){
    float minVal;
    float len;
    Vector3 res;
    bool found = false;

    Vector3 x = rot * Vector3(scale2.x, 0.0f, 0.0f);
    Vector3 y = rot * Vector3(0.0f, scale2.y, 0.0f);
    Vector3 z = rot * Vector3(0.0f, 0.0f, scale2.z);

    Vector3 vecs[8] = {
        pos + x + y + z,
        pos - x + y + z,
        pos + x - y + z,
        pos - x - y + z,
        pos + x + y - z,
        pos - x + y - z,
        pos + x - y - z,
        pos - x - y - z,
    }; 

    for (int i = 0; i < 8; i++){
        if (!found){
            if (CastLength2(vecs[i], dir, scale, res, len)){
                found = true;
                minVal = len;
                result = res;
            }
        }else{
            if (CastLength2(vecs[i], dir, scale, res, len) && len < minVal){
                minVal = len;
                result = res;
            }
        }
    }

    if (!found)
        return false;
    
    minlen = Sqrt(minVal);
    return true;
}

CubeCollider::CubeCollider(Rigidbody* rb) : ACollider(ColliderType::CUBE) {
    this->rb = rb;
}

CubeCollider::~CubeCollider(){}

void CubeCollider::OnCollideCube(const CubeCollider* collider) const{
    Quaternion rot = collider->GetRotation();
    Vector3 pos = (-rot) * (GetPosition() - collider->GetPosition());
    Rigidbody* rb2 = collider->GetRigidbody();
    Vector3 vel = Vector3::zero;
    Vector3 point;
    float len;

    if (rb) vel += rb->velocity;
    if (rb2) vel -= rb2->velocity;
    vel /= rot;

    // 1.2F 是为了防止自旋等运动造成穿模
    if (CastMinLength(pos, vel, collider->scale, scale, GetRotation() / rot, point, len)
            && len <= vel.Magnitude() * Time::GetDeltaTime() * 1.2f){
        vel *= rot;
        // no 1 2 4 => point.mul(world.scale).mul(world.rotation); 针对rb1的受力点
        // point.mul(tarpos.rotation); 针对rb2的受力点
        if (rb){
            if (rb2){
                rb->velocity -= vel;
                rb2->velocity += vel;
                return;
            }
            rb->velocity -= vel * 2.0f;
        }else{
            rb2->velocity += vel * 2.0f;
        }
    }
}

void CubeCollider::OnCollideSphere(const SphereCollider* collider) const{}

void CubeCollider::OnCollideMesh(const MeshCollider* collider) const{}