#ifndef __VIEWOBJECT__
#define __VIEWOBJECT__

#include "list.h"
#include "vecmath.h"
#include "mesh.h"

class Transform;
class AViewObject;
class MeshObject;
class BezierCurveObject;

class Transform {
public:
    enum RotationMode {
        ROT_QUATERNION,
        ROT_EULER_XYZ,
        ROT_EULER_XZY,
        ROT_EULER_YZX,
        ROT_EULER_YXZ,
        ROT_EULER_ZXY,
        ROT_EULER_ZYX
    };

    //TODO 先就只使用这一种，其他的以后补齐
    RotationMode rotationMode = ROT_QUATERNION;

    Vector3 position = Vector3::zero;
    Quaternion rotation = Quaternion::one;
    Vector3 rotationXYZ = Vector3::zero;
    Vector3 scale = Vector3::one;
};

//TODO 3D视口中的可视对象，内部继承后可作为网格体、声源、曲线等
//TODO 正在设计中
class AViewObject {
public:
    Transform transform;

protected:
    AViewObject* parent = NULL;

    List<AViewObject*> children;

public:
    AViewObject();
    virtual ~AViewObject();

    void AddChild(AViewObject* o);
    // 此函数不会回收内存，回收在解构器中进行
    bool DeleteChild(AViewObject* o);
    void EnumChildren(void(*func)(AViewObject*));
    void EnumChildren(void(*func)(AViewObject*, void*), void* user);
    List<AViewObject*>& GetChildren();

    // 具体选择什么应取决于选择模式，计划放置于全局数据中
    // 除了网格体，也应有其它类型对象被选中
    virtual void OnSelect(Vector3 ori, Vector3 dir);
    virtual void OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2);
    virtual void OnSelectUV(Vector2 uv, float err = 0.01f);
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2);

    virtual Mesh* GetMesh();

    virtual void OnRender();
    virtual void OnRenderUVMap();

    virtual void OnTimer(int id);
    virtual void OnChar(char c);
    virtual void OnUnichar(wchar_t c);
    virtual void OnMenuAccel(int id, bool accel);

    virtual void OnAnimationFrame(int frame);
};

class MeshObject : public AViewObject {
private:
    Mesh* mesh;

public:
    MeshObject();
    virtual ~MeshObject() override;

    virtual void OnSelect(Vector3 ori, Vector3 dir) override;
    virtual void OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2) override;
    virtual void OnSelectUV(Vector2 uv, float err = 0.01f) override;
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2) override;

    virtual Mesh* GetMesh() override;

    virtual void OnRender() override;
    virtual void OnRenderUVMap() override;
};

// 大概只是一个测试对象
class BezierCurveObject : public AViewObject {
private:
    Vertex v[4];

public:
    BezierCurveObject();
    virtual ~BezierCurveObject() override;

    virtual void OnSelect(Vector3 ori, Vector3 dir) override;
    virtual void OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2) override;
    virtual void OnSelectUV(Vector2 uv, float err = 0.01f) override;
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2) override;

    virtual void OnRender() override;
    virtual void OnRenderUVMap() override;
};

#endif