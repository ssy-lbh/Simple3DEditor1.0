#include "viewobject.h"

AViewObject::AViewObject(){}

AViewObject::~AViewObject(){
    Free(children);
}

void AViewObject::AddChild(AViewObject* o){
    o->parent = this;
    children.Add(o);
}

bool AViewObject::DeleteChild(AViewObject* o){
    o->parent = NULL;
    return children.Remove(o);
}

void AViewObject::EnumChildren(void(*func)(AViewObject*)){
    children.Foreach(func);
}

void AViewObject::EnumChildren(void(*func)(AViewObject*, void*), void* user){
    children.Foreach<void*>(func, user);
}

void AViewObject::OnSelect(Vector3 ori, Vector3 dir){}
void AViewObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){}
void AViewObject::OnSelectUV(Vector2 uv){}
void AViewObject::OnSelectUV(Vector2 uv1, Vector2 uv2){}

void AViewObject::OnRender(){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnRender();
}

void AViewObject::OnTimer(int id){}
void AViewObject::OnChar(char c){}
void AViewObject::OnUnichar(wchar_t c){}
void AViewObject::OnMenuAccel(int id, bool accel){}

void AViewObject::OnAnimationFrame(int frame){}