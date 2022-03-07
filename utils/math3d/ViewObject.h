#ifndef __UTILS_MATH3D_VIEWOBJECT__
#define __UTILS_MATH3D_VIEWOBJECT__

#include <define.h>

#include <utils/List.h>
#include <utils/gl/GLUtils.h>
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

    RotationMode rotationMode = ROT_EULER_XYZ;

    // 变换顺序: 大小、旋转、位置
    PropertyVector3 position;
    PropertyQuaternion rotation;
    PropertyVector3 rotationXYZ;
    PropertyVector3 scale;
    
    // Object To World
    Matrix4x4 chainMat = Matrix4x4::identity;
    // World To Object
    Matrix4x4 chainInvMat = Matrix4x4::identity;

    Transform();
    ~Transform();

    Quaternion GetRotation();
    Matrix4x4 GetMatrix();
    Matrix4x4 GetInvMatrix();
    void PushMatrix();
    void PushInvMatrix();
    void PopMatrix();

    void SetFrame(float frame);

    void InsertPos(float frame);
    void InsertRot(float frame);
    void InsertScale(float frame);
};

struct RenderOptions {
public:
    bool vertex;
    bool edge;
    bool face;
    bool light;
    bool editor;
    ObjectOperation objOp;
};

class SelectInfo final : public Object {
public:
    Vector3 camPos;
    Quaternion camDir;
    Vector2 zBound;
    GLRect rect;

    SelectInfo();
    ~SelectInfo();

    bool Inside(Vector3 pos) const;
};

enum class ViewObjectType {
    OBJECT_DEFAULT,
    OBJECT_MESH,
    OBJECT_BEZIER_CURVE,
    OBJECT_POINT_LIGHT,
    OBJECT_AUDIO_SOURCE,
    OBJECT_AUDIO_LISTENER,
    OBJECT_CAMERA
};

class AViewObject : public Object {
public:
    Transform transform;
    WString name;

    bool unfold = true;

protected:
    AViewObject* parent = NULL;
    List<AViewObject*> children;

    const ViewObjectType type = ViewObjectType::OBJECT_DEFAULT;

    AViewObject(ViewObjectType type);
    AViewObject(const wchar_t* name, ViewObjectType type);
    AViewObject(WString name, ViewObjectType type);

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

    ViewObjectType GetType();
    AViewObject* GetParent();
    void SetParent(AViewObject* o);
    bool HasAncestor(AViewObject* o);

    Matrix4x4 GetParentChainMat();
    Matrix4x4 GetParentChainInvMat();
    Point3 GetWorldPos();
    void SetWorldPos(Point3 pos);
    
    // 用字符'.'分隔各级对象名称
    AViewObject* QueryObject(WString path);

    // 具体选择什么应取决于选择模式，计划放置于全局数据中
    // 除了网格体，也应有其它类型对象被选中
    // 已实现的父类方法中，都是对子对象的遍历调用
    //TODO 待实现对象、边、面选择
    virtual void OnSelect(Vector3 ori, Vector3 dir);
    virtual void OnSelect(const SelectInfo* info);
    virtual void OnSelectUV(Vector2 uv, float err = 0.01f);
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2);

    virtual Mesh* GetMesh();

    void OnChainRender(const RenderOptions* options);
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
    virtual void OnSelect(const SelectInfo* info) override;
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
    virtual void OnSelect(const SelectInfo* info) override;
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
    virtual void OnSelect(const SelectInfo* info) override;

    virtual void OnRender(const RenderOptions* options) override;

    virtual void OnTimer(int id) override;

    void UpdateLight();
};

class AudioSourceObject final : public AViewObject {
private:
    static int REPLAY_ERROR;

    uint alSrc;
    uint alBuf;

    /* size in samples */
    int alAudioSize;
    /* length in seconds */
    int alAudioLen;
    /* size in bytes */
    int alSampleSize;
    /* number of channels */
    int alChannels;
    /* audio data */
    void* alAudioData;
    int alAudioFreq;
    int alAudioOffset;

    Point3 recPos;
    float recTime;
    bool dopplerEffect = true;

    AudioPlayerWindow* window = NULL;

public:
    // 输入的数据内部引用，自动回收，不能调用后释放data所在内存
    AudioSourceObject(uenum format, char* data, int size, int freq);
    virtual ~AudioSourceObject() override;

    virtual void OnTimer(int id) override;

    // 使用渲染回调获取最新位置信息，并更新AudioListener
    virtual void OnRender(const RenderOptions* options) override;

    char* GetData();
    int GetSize();
    int GetLength();
    int GetSampleSize();
    int GetChannelCount();
    int GetFrequency();
    int GetOffset();
    bool IsLoop();
    float GetGain();
    AudioPlayerWindow* GetWindowRef();

    void SetOffset(int offset);
    void SetLoop(bool loop);
    void SetGain(float gain);
    void SetWindowRef(AudioPlayerWindow* window);

    void Play();
    void Stop();
    bool IsPlaying();

    void SetPosv3(Point3 value);
    void SetVelocityv3(Vector3 value);
    void SetPosAutoVelv3(Point3 value);
    
    void SetDopplerEffect(bool on);
    bool HasDopplerEffect();
};

//TODO 可修改为附着于一个物体，以transform确定位置
class AudioListenerObject final : public AViewObject {
private:
    Point3 recPos;
    float recTime;
    bool dopplerEffect = true;

public:
    AudioListenerObject();
    virtual ~AudioListenerObject() override;

    // 使用渲染回调获取最新位置信息，并更新AudioListener
    virtual void OnRender(const RenderOptions* options) override;

    void SetPosv3(Point3 value);
    void SetVelocityv3(Vector3 value);
    void SetPosAutoVelv3(Point3 value);
    
    void SetDopplerEffect(bool on);
    bool HasDopplerEffect();
};

class CameraObject final : public AViewObject {
private:
    static const float SCALE;

    Vertex pos;
    Vertex lookAt;

public:
    CameraObject();
    virtual ~CameraObject() override;

    virtual void OnSelect(Vector3 ori, Vector3 dir) override;
    virtual void OnSelect(const SelectInfo* info) override;

    virtual void OnRender(const RenderOptions* options) override;
};

#endif