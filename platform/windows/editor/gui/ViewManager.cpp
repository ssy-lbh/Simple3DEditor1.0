#include <editor/gui/ViewManager.h>

#include <gl/gl.h>

#include <utils/os/Thread.h>
#include <utils/os/AppFrame.h>

ViewManager::ViewManager(AppFrame* frame) : frame(frame) {}
ViewManager::~ViewManager(){}

ViewManager* ViewManager::GetLocalInst(){
    return (ViewManager*)ThreadLocal::Get(THREAD_LOCAL_VIEWMGR);
}

void ViewManager::Reset(){
    RECT cliRect;
    GLRect rect;

    rects.Clear();

    GetClientRect(frame->hWnd, &cliRect);
    rect.left = cliRect.left; rect.right = cliRect.right;
    rect.top = cliRect.bottom; rect.bottom = cliRect.top;

    PushView(rect);
}

void ViewManager::PushView(GLRect rect){
    rects.Add(rect);
    SetView(rect);
}

void ViewManager::PopView(){
    rects.RemoveBack();
    SetView(rects.GetBack());
}

void ViewManager::SetView(GLRect rect){
    curRect = rect;
    glViewport(curRect.left, curRect.bottom, curRect.right - curRect.left, curRect.top - curRect.bottom);
    glScissor(curRect.left, curRect.bottom, curRect.right - curRect.left, curRect.top - curRect.bottom);
}

GLRect ViewManager::GetRect(){
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

GLRect ViewManager::CalcChildRect(GLRect ratio){
    return curRect.ChildRect(ratio);
}

void ViewManager::PushChildView(GLRect ratio){
    PushView(curRect.ChildRect(ratio));
}

void ViewManager::SetScissor(GLRect rect){
    glScissor(rect.left, rect.bottom, rect.right - rect.left, rect.top - rect.bottom);
}

void ViewManager::SetChildScissor(GLRect ratio){
    SetScissor(curRect.ChildRect(ratio));
}

void ViewManager::ResetScissor(){
    SetScissor(curRect);
}