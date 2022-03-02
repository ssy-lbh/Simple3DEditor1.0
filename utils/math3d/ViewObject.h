#ifndef __UTILS_MATH3D_VIEWOBJECT__
#define __UTILS_MATH3D_VIEWOBJECT__

#include <define.h>

#include <utils/List.h>
#include <utils/math3d/Geometry.h>
#include <utils/math3d/LinearAlgebra.h>
#include <utils/math3d/Property.h>
#include <utils/String.h>

class Transform final : public Object {
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
    RotationMode rotationMode = ROT_EULER_XYZ;

    // 变换顺序: 大小、旋转、位置
    PropertyVector3 position;
    PropertyQuaternion rotation;
    PropertyVector3 rotationXYZ;
    PropertyVector3 scale;

    Transform();
    ~Transform();

    Quaternion GetRotation();
    Matrix4x4 GetMatrix();
    Matrix4x4 GetInvMatrix();
    void PushMatrix();
    void PushInvMatrix();
    void PopMatrix();

    void SetFrame(float frame);
};

class RenderOptions : public Object {
public:
    bool light = false;
};

//TODO 3D视口中的可视对象，内部继承后可作为网格体、声源、曲线等
//TODO 正在设计中
class AViewObject : public Object {
public:
    Transform transform;
    WString name;

    bool unfold = true;

protected:
    AViewObject* parent = NULL;

    List<AViewObject*> children;

public:
    AViewObject();
    AViewObject(const wchar_t* name);
    AViewObject(WString name);
    virtual ~AViewObject();

    void AddChild(AViewObject* o);
    // 此函数不会回收内存，回收在解构器中进行
    bool DeleteChild(AViewObject* o);
    void EnumChildren(void(*func)(AViewObject*));
    void EnumChildren(void(*func)(AViewObject*, void*), void* user);
    List<AViewObject*>& GetChildren();

    AViewObject* GetParent();
    void SetParent(AViewObject* o);

    Matrix4x4 GetObjectToWorldMatrix();
    Matrix4x4 GetWorldToObjectMatrix();

    // 具体选择什么应取决于选择模式，计划放置于全局数据中
    // 除了网格体，也应有其它类型对象被选中
    // 已实现的父类方法中，都是对子对象的遍历调用
    virtual void OnSelect(Vector3 ori, Vector3 dir);
    virtual void OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2);
    virtual void OnSelectUV(Vector2 uv, float err = 0.01f);
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2);

    virtual Mesh* GetMesh();

    virtual void OnRender(const RenderOptions* options);
    virtual void OnRenderUVMap();

    virtual void OnTimer(int id);
    virtual void OnChar(char c);
    virtual void OnUnichar(wchar_t c);
    virtual void OnMenuAccel(int id, bool accel);

    virtual void OnAnimationFrame(float frame);
};

class MeshObject final : public AViewObject {
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

    virtual void OnRender(const RenderOptions* options) override;
    virtual void OnRenderUVMap() override;
};

// 大概只是一个测试对象
class BezierCurveObject final : public AViewObject {
private:
    Vertex v[4];

public:
    BezierCurveObject();
    virtual ~BezierCurveObject() override;

    virtual void OnSelect(Vector3 ori, Vector3 dir) override;
    virtual void OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2) override;
    virtual void OnSelectUV(Vector2 uv, float err = 0.01f) override;
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2) override;

    virtual void OnRender(const RenderOptions* options) override;
    virtual void OnRenderUVMap() override;
};

class PointLightObject final : public AViewObject {
private:
    Vertex v;
    uenum light;

public:
    PointLightObject();
    virtual ~PointLightObject() override;

    virtual void OnSelect(Vector3 ori, Vector3 dir) override;
    virtual void OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2) override;

    virtual void OnRender(const RenderOptions* options) override;

    virtual void OnTimer(int id) override;

    void UpdateLight();
};

class AudioListenerObject final : public AViewObject {
private:
    Vertex v;

public:
    AudioListenerObject();
    virtual ~AudioListenerObject() override;

    virtual void OnSelect(Vector3 ori, Vector3 dir) override;
    virtual void OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2) override;

    virtual void OnRender(const RenderOptions* options) override;

    virtual void OnTimer(int id) override;
};

#endif