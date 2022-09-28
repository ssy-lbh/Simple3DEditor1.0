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

//! 注意:暂定clazz必须是[所有namespace用'::'隔开]+[类名]严格区分，ID与符号表同步唯一
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

    // 被销毁时，子对象全部已销毁
    Delegate<AViewObject*> onDestroy;

    AViewObject();
    AViewObject(const wchar_t* name);
    AViewObject(WString name);
    virtual ~AViewObject();

    virtual void AddChild(AViewObject* o);
    // 此函数不会回收内存，回收在解构器中进行
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
    
    // 用字符'.'分隔各级对象名称
    // 计划这里内部的实现以后改为HashMap加速
    AViewObject* QueryObject(WString path);

    bool IsFocus();

    // 具体选择什么应取决于选择模式，计划放置于全局数据中
    // 除了网格体，也应有其它类型对象被选中
    // 已实现的父类方法中，都是对子对象的遍历调用
    //TODO 待实现对象、边、面选择
    virtual void OnSelect(Point3 ori, Vector3 dir);
    virtual void OnSelect(const SelectInfo* info);
    virtual void OnSelectUV(Vector2 uv, float err = 0.01f);
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2);

    virtual void OnChainRender();
    // 基于OpenGL的渲染，如果渲染全过程已做好抽象可换用API
    virtual void OnRender();
    virtual void OnRenderUVMap();

    virtual void OnTimer(int id);
    virtual void OnChar(char c);
    virtual void OnUnichar(wchar_t c);
    //TODO 改为每一次渲染之前调用，提前告知比率等信息
    virtual void OnResize(Vector2 size);
    // 链式传递鼠标位移请使用3D
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
    // 返回是否在组件上方
    virtual bool OnHit2D(Point2 pos);
    virtual void OnFocus();
    virtual void OnKillFocus();
    virtual void OnMouseWheel(int delta);
    virtual void OnMenuAccel(int id, bool accel);
    virtual void OnDropFileA(const char* path, uint len);
    virtual void OnDropFileW(const wchar_t* path, uint len);

    virtual void OnAnimationFrame(float frame);

    // 窗口序列化时应先o["id"] = OBJECT_ID，这样才能在反序列化时识别窗口
    virtual void Serialize(nlohmann::json& o) override;
    // 除了OBJECT_ID，其他数据按顺序依次读取就行
    virtual void Deserialize(nlohmann::json& o) override;
};

}

#endif