#ifndef __EDITOR_MAIN_VIEWOBJECT__
#define __EDITOR_MAIN_VIEWOBJECT__

#include <define.h>

#include <utils/List.h>
#include <utils/math3d/Geometry.h>
#include <utils/math3d/LinearAlgebra.h>
#include <utils/String.h>
#include <editor/main/Transform.h>

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
    OBJECT_BEZIER_CURVE,
    OBJECT_POINT_LIGHT,
    OBJECT_AUDIO_SOURCE,
    OBJECT_AUDIO_LISTENER,
    OBJECT_CAMERA,
    GUI_MANAGER,
    GUI_ICON_BUTTON,
    GUI_EDIT_A,
    GUI_EDIT_W
};

class AViewObject : public Object {
public:
    Transform transform;
    WString name;

    bool unfold = true;

protected:
    bool focus = false;

    Vector2 cliSize;
    Point3 cursorOri;
    Vector3 cursorDir;

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

    virtual void AddChild(AViewObject* o);
    // 此函数不会回收内存，回收在解构器中进行
    virtual bool DeleteChild(AViewObject* o);
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

    void OnChainRender();
    // 基于OpenGL的渲染，如果渲染全过程已做好抽象可换用API
    virtual void OnRender();
    virtual void OnRenderUVMap();

    // 要求调用时OpenGL或者其他渲染API上下文存在
    virtual void OnCreate();
    virtual void OnClose();
    virtual void OnTimer(int id);
    virtual void OnChar(char c);
    virtual void OnUnichar(wchar_t c);
    virtual void OnResize(Vector2 size);
    virtual void OnMouseMove(Point3 ori, Vector3 dir);
    virtual void OnLeftDown(Point3 ori, Vector3 dir);
    virtual void OnLeftUp(Point3 ori, Vector3 dir);
    virtual void OnLeftDrag(Vector3 delta, Vector3 dir);
    virtual void OnRightDown(Point3 ori, Vector3 dir);
    virtual void OnRightUp(Point3 ori, Vector3 dir);
    virtual void OnRightDrag(Vector3 delta, Vector3 dir);
    virtual void OnFocus();
    virtual void OnKillFocus();
    virtual void OnMouseWheel(int delta);
    virtual void OnMenuAccel(int id, bool accel);
    virtual void OnDropFileA(const char* path);
    virtual void OnDropFileW(const wchar_t* path);
    virtual bool OnHit(Point3 ori, Vector3 dir);

    virtual void OnAnimationFrame(float frame);
};

#endif