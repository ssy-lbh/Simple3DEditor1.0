#include <editor/main/ViewObject.h>

#include <lib/openal/al.h>

#include <main.h>
#include <res.h>
#include <editor/MainWindow.h>
#include <editor/AudioPlayerWindow.h>
#include <util/AudioUtils.h>
#include <util/os/Time.h>
#include <util/math3d/Math.h>
#include <util/math3d/Mesh.h>
#include <util/gl/GLSimplified.h>
#include <util/gl/GLLights.h>
#include <util/gl/GLUtils.h>

#include <lib/json/nlohmann/json.hpp>

namespace simple3deditor {

SelectInfo::SelectInfo(){}
SelectInfo::~SelectInfo(){}

bool SelectInfo::Inside(Vector3 pos) const{
    Vector3 lookPos = (-camDir) * (pos - camPos);
    if (lookPos.y < zBound.x || lookPos.y > zBound.y)
        return false;
    return rect.Inside(Vector2(lookPos.x, lookPos.z) / lookPos.y);
}

OBJECT_INFO_DECL(simple3deditor::AViewObject, L"");

AViewObject::AViewObject() : name(L"Object") {}
AViewObject::AViewObject(ViewObjectType type) : name(L"Object"), type(type) {}
AViewObject::AViewObject(const wchar_t* name) : name(name) {}
AViewObject::AViewObject(const wchar_t* name, ViewObjectType type) : name(name), type(type) {}
AViewObject::AViewObject(WString name) : name(name){}
AViewObject::AViewObject(WString name, ViewObjectType type) : name(name), type(type) {}

AViewObject::~AViewObject(){
    Free(children);
    onDestroy(this);
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

void AViewObject::EnumChildren(std::function<void(AViewObject*)> func){
    children.Foreach(func);
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
    return transform.chainMat.GetTranslation();
}

void AViewObject::SetWorldPos(Point3 pos){
    transform.position.Set(parent == NULL ? pos : parent->transform.chainInvMat * pos);
}

Quaternion AViewObject::GetWorldRot(){
    return parent == NULL ? transform.GetRotation() : transform.GetRotation() * parent->GetWorldRot();
}

void AViewObject::SetWorldRot(Quaternion rot){
    transform.SetRotation(parent == NULL ? rot : rot / parent->GetWorldRot());
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
    const RenderOptions* options = &LocalData::GetLocalInst()->renderOptions;
    if (Main::data->curObject == this && options->editor && Main::data->selType == SelectionType::SELECT_OBJECT){
        glDisable(GL_LIGHTING);
        glEnable(GL_POINT_SMOOTH);
        glPointSize(10.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertexv3(Point3::zero);
        glEnd();
        glDisable(GL_POINT_SMOOTH);

        // 默认MOVE
        switch (options->objOp){
        case ObjectOperation::MOVE:
            glColor3f(1.0f, 0.0f, 0.0f);
            GLUtils::Draw3DArrow(Point3::zero, Vector3::right, 0.3f, 0.3f, 5.0f);
            glColor3f(0.0f, 1.0f, 0.0f);
            GLUtils::Draw3DArrow(Point3::zero, Vector3::forward, 0.3f, 0.3f, 5.0f);
            glColor3f(0.0f, 0.0f, 1.0f);
            GLUtils::Draw3DArrow(Point3::zero, Vector3::up, 0.3f, 0.3f, 5.0f);
            break;
        case ObjectOperation::ROTATE:
            break;
        case ObjectOperation::SCALE:
            break;
        }
    }
}

void AViewObject::OnRenderUVMap(){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnRenderUVMap();
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
    for (size_t i = 0; i < len; i++)
        children[i]->OnResize(size);
}

void AViewObject::OnMouseMove(Point3 ori, Vector3 dir){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnMouseMove(ori, dir);
}

void AViewObject::OnLeftDown(Point3 ori, Vector3 dir){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnLeftDown(ori, dir);
}

void AViewObject::OnLeftUp(Point3 ori, Vector3 dir){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnLeftUp(ori, dir);
}

void AViewObject::OnRightDown(Point3 ori, Vector3 dir){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnRightDown(ori, dir);
}

void AViewObject::OnRightUp(Point3 ori, Vector3 dir){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnRightUp(ori, dir);
}

void AViewObject::OnMouseMove2D(Point2 pos){}
void AViewObject::OnLeftDown2D(Point2 pos){}
void AViewObject::OnLeftUp2D(Point2 pos){}
void AViewObject::OnLeftDrag2D(Vector2 dir){}
void AViewObject::OnRightDown2D(Point2 pos){}
void AViewObject::OnRightUp2D(Point2 pos){}
void AViewObject::OnRightDrag2D(Vector2 dir){}
bool AViewObject::OnHit2D(Point2 pos){ return false; }

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

void AViewObject::OnDropFileA(const char* path, uint len){
    size_t size = children.Size();
    for (size_t i = 0; i < size; i++)
        children[i]->OnDropFileA(path, len);
}

void AViewObject::OnDropFileW(const wchar_t* path, uint len){
    size_t size = children.Size();
    for (size_t i = 0; i < size; i++)
        children[i]->OnDropFileW(path, len);
}

void AViewObject::OnAnimationFrame(float frame){
    size_t len = children.Size();
    transform.SetFrame(frame);
    for (size_t i = 0; i < len; i++)
        children[i]->OnAnimationFrame(frame);
}

void AViewObject::Serialize(nlohmann::json& o){
    o["id"] = OBJECT_ID;
    o["name"] = String(name).GetString();
    nlohmann::json ch = nlohmann::json::array();
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++){
        nlohmann::json o;
        children[i]->Serialize(o);
        ch.push_back(o);
    }
    o["children"] = ch;
}

void AViewObject::Deserialize(nlohmann::json& o){
    name = String(o.value("name", "object").c_str());
    nlohmann::json ch = o["children"];
    if (ch.is_array()){
        for (int i = 0; ch[i].is_null() == false; i++)
            AddChild(Main::data->ConstructObject(ch[i]));
    }
}

}