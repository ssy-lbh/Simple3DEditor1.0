#include <editor/main/ViewObject.h>

#include <lib/openal/al.h>

#include <main.h>
#include <res.h>
#include <editor/MainWindow.h>
#include <editor/AudioPlayerWindow.h>
#include <utils/AudioUtils.h>
#include <utils/os/Time.h>
#include <utils/math3d/Math.h>
#include <utils/math3d/Mesh.h>
#include <utils/gl/GLSimplified.h>
#include <utils/gl/GLLights.h>
#include <utils/gl/GLUtils.h>

SelectInfo::SelectInfo(){}
SelectInfo::~SelectInfo(){}

bool SelectInfo::Inside(Vector3 pos) const{
    Vector3 lookPos = (-camDir) * (pos - camPos);
    if (lookPos.y < zBound.x || lookPos.y > zBound.y)
        return false;
    return rect.Inside(Vector2(lookPos.x, lookPos.z) / lookPos.y);
}

AViewObject::AViewObject() : name(L"Object") {}
AViewObject::AViewObject(ViewObjectType type) : name(L"Object"), type(type) {}
AViewObject::AViewObject(const wchar_t* name) : name(name) {}
AViewObject::AViewObject(const wchar_t* name, ViewObjectType type) : name(name), type(type) {}
AViewObject::AViewObject(WString name) : name(name){}
AViewObject::AViewObject(WString name, ViewObjectType type) : name(name), type(type) {}

AViewObject::~AViewObject(){
    Free(children);
    if (parent)
        parent->DeleteChild(this);
}

void AViewObject::AddChild(AViewObject* o){
    if (o->parent)
        o->parent->DeleteChild(o);
    o->parent = this;
    children.Add(o);
}

bool AViewObject::DeleteChild(AViewObject* o){
    if (o->parent != this)
        return false;
    o->parent = NULL;
    return children.Remove(o);
}

void AViewObject::EnumChildren(void(*func)(AViewObject*)){
    children.Foreach(func);
}

void AViewObject::EnumChildren(void(*func)(AViewObject*, void*), void* user){
    children.Foreach<void*>(func, user);
}

ViewObjectType AViewObject::GetType(){
    return type;
}

AViewObject* AViewObject::GetParent(){
    return parent;
}

void AViewObject::SetParent(AViewObject* o){
    if (parent)
        parent->DeleteChild(this);
    o->AddChild(this);
}

bool AViewObject::HasAncestor(AViewObject* o){
    if (this == o)
        return true;
    return (parent == NULL ? false : parent->HasAncestor(o));
}

Matrix4x4 AViewObject::GetParentChainMat(){
    return (parent == NULL ? Matrix4x4::identity : parent->transform.chainMat);
}

Matrix4x4 AViewObject::GetParentChainInvMat(){
    return (parent == NULL ? Matrix4x4::identity : parent->transform.chainInvMat);
}

Point3 AViewObject::GetWorldPos(){
    return transform.chainMat * Point3::zero;
}

void AViewObject::SetWorldPos(Point3 pos){
    transform.position.Set(parent == NULL ? pos : parent->transform.chainInvMat * pos);
}

AViewObject* AViewObject::QueryObject(WString path){
    size_t pos = path.FindChar('.');
    WString name = path.SubString(0, pos);
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        if (children[i]->name == name)
            return (pos == -1 ? children[i] : children[i]->QueryObject(path.SubString(pos + 1)));
    return NULL;
}

List<AViewObject*>& AViewObject::GetChildren(){
    return children;
}

void AViewObject::OnSelect(Point3 ori, Vector3 dir){}
void AViewObject::OnSelect(const SelectInfo* info){}
void AViewObject::OnSelectUV(Vector2 uv, float err){}
void AViewObject::OnSelectUV(Vector2 uv1, Vector2 uv2){}

void AViewObject::OnChainRender(){
    size_t len = children.Size();

    if (parent){
        transform.chainMat = parent->transform.chainMat * transform.GetMatrix();
        transform.chainInvMat = transform.GetInvMatrix() * parent->transform.chainInvMat;
    }else{
        transform.chainMat = transform.GetMatrix();
        transform.chainInvMat = transform.GetInvMatrix();
    }

    transform.PushMatrix();
    for (size_t i = 0; i < len; i++)
        children[i]->OnChainRender();
    OnRender();
    transform.PopMatrix();
}

void AViewObject::OnRender(){
    if (Main::data->selType == SelectionType::SELECT_OBJECT && Main::data->curObject == this){
        glDisable(GL_LIGHTING);
        glEnable(GL_POINT_SMOOTH);
        glPointSize(10.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertexv3(Point3::zero);
        glEnd();
        glDisable(GL_POINT_SMOOTH);

        // 默认MOVE
        glColor3f(1.0f, 0.0f, 0.0f);
        GLUtils::Draw3DArrow(Point3::zero, Vector3::right, 0.3f, 0.3f, 5.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        GLUtils::Draw3DArrow(Point3::zero, Vector3::forward, 0.3f, 0.3f, 5.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        GLUtils::Draw3DArrow(Point3::zero, Vector3::up, 0.3f, 0.3f, 5.0f);
    }
}

void AViewObject::OnRenderUVMap(){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnRenderUVMap();
}

void AViewObject::OnCreate(){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnCreate();
}

void AViewObject::OnClose(){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnClose();
}

void AViewObject::OnTimer(int id){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnTimer(id);
}

void AViewObject::OnChar(char c){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnChar(c);
}

void AViewObject::OnUnichar(wchar_t c){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnUnichar(c);
}

void AViewObject::OnResize(Vector2 size){
    size_t len = children.Size();
    cliSize = size;
    for (size_t i = 0; i < len; i++)
        children[i]->OnResize(size);
}

void AViewObject::OnMouseMove(Point3 ori, Vector3 dir){
    size_t len = children.Size();
    cursorOri = ori; cursorDir = dir;
    for (size_t i = 0; i < len; i++)
        children[i]->OnMouseMove(ori, dir);
}

void AViewObject::OnLeftDown(Point3 ori, Vector3 dir){
    size_t len = children.Size();
    cursorOri = ori; cursorDir = dir;
    for (size_t i = 0; i < len; i++)
        children[i]->OnLeftDown(ori, dir);
}

void AViewObject::OnLeftUp(Point3 ori, Vector3 dir){
    size_t len = children.Size();
    cursorOri = ori; cursorDir = dir;
    for (size_t i = 0; i < len; i++)
        children[i]->OnLeftUp(ori, dir);
}

void AViewObject::OnLeftDrag(Vector3 delta, Vector3 dir){
    size_t len = children.Size();
    cursorDir = dir;
    for (size_t i = 0; i < len; i++)
        children[i]->OnLeftDrag(delta, dir);
}

void AViewObject::OnRightDown(Point3 ori, Vector3 dir){
    size_t len = children.Size();
    cursorOri = ori; cursorDir = dir;
    for (size_t i = 0; i < len; i++)
        children[i]->OnRightDown(ori, dir);
}

void AViewObject::OnRightUp(Point3 ori, Vector3 dir){
    size_t len = children.Size();
    cursorOri = ori; cursorDir = dir;
    for (size_t i = 0; i < len; i++)
        children[i]->OnRightUp(ori, dir);
}

void AViewObject::OnRightDrag(Vector3 delta, Vector3 dir){
    size_t len = children.Size();
    cursorDir = dir;
    for (size_t i = 0; i < len; i++)
        children[i]->OnRightDrag(delta, dir);
}

void AViewObject::OnFocus(){
    size_t len = children.Size();
    focus = true;
    for (size_t i = 0; i < len; i++)
        children[i]->OnFocus();
}

void AViewObject::OnKillFocus(){
    size_t len = children.Size();
    focus = false;
    for (size_t i = 0; i < len; i++)
        children[i]->OnKillFocus();
}

void AViewObject::OnMouseWheel(int delta){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnMouseWheel(delta);
}

void AViewObject::OnMenuAccel(int id, bool accel){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnMenuAccel(id, accel);
}

void AViewObject::OnDropFileA(const char* path){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnDropFileA(path);
}

void AViewObject::OnDropFileW(const wchar_t* path){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnDropFileW(path);
}

bool AViewObject::OnHit(Point3 ori, Vector3 dir){
    return false;
}

void AViewObject::OnAnimationFrame(float frame){
    size_t len = children.Size();
    transform.SetFrame(frame);
    for (size_t i = 0; i < len; i++)
        children[i]->OnAnimationFrame(frame);
}