#ifndef __EDITOR_MAIN_VIEWOBJECT__
#define __EDITOR_MAIN_VIEWOBJECT__

#include <define.h>

#include <io/Serializable.h>
#include <util/List.h>
#include <util/Delegate.h>
#include <util/math3d/Geometry.h>
#include <util/math3d/LinearAlgebra.h>
#include <util/String.h>
#include <editor/main/Transform.h>

namespace simple3deditor {

class SelectInfo final : public Object {
public:
    Vector3 camPos;
    Quaternion camDir;
    Vector2 zBound;
    Rect rect;

    SelectInfo();
    ~SelectInfo();

    bool Inside(Vector3 pos) const;
};

enum class ViewObjectType {
    OBJECT_DEFAULT,
    OBJECT_MESH,
    OBJECT_CUBIC_BEZIER_CURVE,
    OBJECT_SQUARE_BEZIER_CURVE,
    OBJECT_POINT_LIGHT,
    OBJECT_AUDIO_SOURCE,
    OBJECT_AUDIO_LISTENER,
    OBJECT_CAMERA,
    OBJECT_WINDOW,
    
    OBJECT_GUI_MESH,
    OBJECT_GUI,
    OBJECT_GUI_MANAGER,
    OBJECT_GUI_ROUND_BUTTON,
    OBJECT_GUI_ICON_BUTTON,
    OBJECT_GUI_EDIT_A,
    OBJECT_GUI_EDIT_W,
    OBJECT_HORIZONTAL_PROGRESSBAR,
    OBJECT_VERTICAL_PROGRESSBAR
};

#define OBJECT_INFO_DEF()\
    static const char* OBJECT_ID;\
    static const wchar_t* OBJECT_DISPLAY_NAME

//! ע��:�ݶ�clazz������[����namespace��'::'����]+[����]�ϸ����֣�ID����ű�ͬ��Ψһ
#define OBJECT_INFO_DECL(clazz, displayName)\
    const char* clazz::OBJECT_ID = #clazz;\
    const wchar_t* clazz::OBJECT_DISPLAY_NAME = displayName

class AViewObject : public Object, public IMemorable {
public:
    Transform transform;
    WString name;

    bool unfold = true;

protected:
    bool focus = false;

    AViewObject* parent = NULL;
    List<AViewObject*> children;

    const ViewObjectType type = ViewObjectType::OBJECT_DEFAULT;

    AViewObject(ViewObjectType type);
    AViewObject(const wchar_t* name, ViewObjectType type);
    AViewObject(WString name, ViewObjectType type);

public:
    OBJECT_INFO_DEF();

    // ������ʱ���Ӷ���ȫ��������
    Delegate<AViewObject*> onDestroy;

    AViewObject();
    AViewObject(const wchar_t* name);
    AViewObject(WString name);
    virtual ~AViewObject();

    virtual void AddChild(AViewObject* o);
    // �˺�����������ڴ棬�����ڽ⹹���н���
    virtual bool DeleteChild(AViewObject* o);
    virtual void EnumChildren(std::function<void(AViewObject*)> func);
    List<AViewObject*>& GetChildren();

    ViewObjectType GetType();
    AViewObject* GetParent();
    void SetParent(AViewObject* o);
    bool HasAncestor(AViewObject* o);

    Matrix4x4 GetParentChainMat();
    Matrix4x4 GetParentChainInvMat();
    Point3 GetWorldPos();
    void SetWorldPos(Point3 pos);
    Quaternion GetWorldRot();
    void SetWorldRot(Quaternion rot);
    
    // ���ַ�'.'�ָ�������������
    // �ƻ������ڲ���ʵ���Ժ��ΪHashMap����
    AViewObject* QueryObject(WString path);

    bool IsFocus();

    // ����ѡ��ʲôӦȡ����ѡ��ģʽ���ƻ�������ȫ��������
    // ���������壬ҲӦ���������Ͷ���ѡ��
    // ��ʵ�ֵĸ��෽���У����Ƕ��Ӷ���ı�������
    //TODO ��ʵ�ֶ��󡢱ߡ���ѡ��
    virtual void OnSelect(Point3 ori, Vector3 dir);
    virtual void OnSelect(const SelectInfo* info);
    virtual void OnSelectUV(Vector2 uv, float err = 0.01f);
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2);

    virtual void OnChainRender();
    // ����OpenGL����Ⱦ�������Ⱦȫ���������ó���ɻ���API
    virtual void OnRender();
    virtual void OnRenderUVMap();

    virtual void OnTimer(int id);
    virtual void OnChar(char c);
    virtual void OnUnichar(wchar_t c);
    //TODO ��Ϊÿһ����Ⱦ֮ǰ���ã���ǰ��֪���ʵ���Ϣ
    virtual void OnResize(Vector2 size);
    // ��ʽ�������λ����ʹ��3D
    virtual void OnMouseMove(Point3 ori, Vector3 dir);
    virtual void OnLeftDown(Point3 ori, Vector3 dir);
    virtual void OnLeftUp(Point3 ori, Vector3 dir);
    virtual void OnRightDown(Point3 ori, Vector3 dir);
    virtual void OnRightUp(Point3 ori, Vector3 dir);
    virtual void OnMouseMove2D(Point2 pos);
    virtual void OnLeftDown2D(Point2 pos);
    virtual void OnLeftUp2D(Point2 pos);
    virtual void OnLeftDrag2D(Vector2 dir);
    virtual void OnRightDown2D(Point2 pos);
    virtual void OnRightUp2D(Point2 pos);
    virtual void OnRightDrag2D(Vector2 dir);
    // �����Ƿ�������Ϸ�
    virtual bool OnHit2D(Point2 pos);
    virtual void OnFocus();
    virtual void OnKillFocus();
    virtual void OnMouseWheel(int delta);
    virtual void OnMenuAccel(int id, bool accel);
    virtual void OnDropFileA(const char* path, uint len);
    virtual void OnDropFileW(const wchar_t* path, uint len);

    virtual void OnAnimationFrame(float frame);

    // �������л�ʱӦ��o["id"] = OBJECT_ID�����������ڷ����л�ʱʶ�𴰿�
    virtual void Serialize(nlohmann::json& o) override;
    // ����OBJECT_ID���������ݰ�˳�����ζ�ȡ����
    virtual void Deserialize(nlohmann::json& o) override;
};

}

#endif