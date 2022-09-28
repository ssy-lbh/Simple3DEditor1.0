#include <editor/main/ViewManager.h>

#include <gl/gl.h>

#include <util/os/Thread.h>
#include <util/os/AppFrame.h>

namespace simple3deditor {

ViewManager::ViewManager(AppFrame* frame) : frame(frame) {}
ViewManager::~ViewManager(){}

ViewManager* ViewManager::GetLocalInst(){
    return static_cast<ViewManager*>(ThreadLocal::Get(THREAD_LOCAL_VIEWMGR));
}

void ViewManager::Reset(){
    rects.Clear();
    PushView(frame->GetClientRect());
}

void ViewManager::PushView(Rect rect){
    rects.Add(rect);
    SetView(rect);
}

void ViewManager::PopView(){
    rects.RemoveBack();
    SetView(rects.GetBack());
}

void ViewManager::SetView(Rect rect){
    curRect = rect;
    glViewport(curRect.left, curRect.bottom, curRect.right - curRect.left, curRect.top - curRect.bottom);
    glScissor(curRect.left, curRect.bottom, curRect.right - curRect.left, curRect.top - curRect.bottom);
}

Rect ViewManager::GetRect(){
    return curRect;
}

float ViewManager::GetWidth(){
    return curRect.GetWidth();
}

float ViewManager::GetHeight(){
    return curRect.GetHeight();
}

float ViewManager::GetAspect(){
    return curRect.GetAspect();
}

void ViewManager::EnableScissor(){
    glEnable(GL_SCISSOR_TEST);
    SetScissor(curRect);
}

void ViewManager::DisableScissor(){
    glDisable(GL_SCISSOR_TEST);
}

Vector2 ViewManager::GetSize(){
    return Vector2(curRect.right - curRect.left, curRect.top - curRect.bottom);
}

Rect ViewManager::CalcChildRect(Rect ratio){
    return curRect.ChildRect(ratio);
}

void ViewManager::PushChildView(Rect ratio){
    PushView(curRect.ChildRect(ratio));
}

void ViewManager::SetScissor(Rect rect){
    glScissor(rect.left, rect.bottom, rect.right - rect.left, rect.top - rect.bottom);
}

void ViewManager::SetChildScissor(Rect ratio){
    SetScissor(curRect.ChildRect(ratio));
}

void ViewManager::ResetScissor(){
    SetScissor(curRect);
}

}