#include "uimgr.h"

#include <windows.h>
#include <windowsx.h>

#include <gl/gl.h>

UIManager::UIManager(){}

UIManager::~UIManager(){
    Free(buttons);
}

void UIManager::AddButton(IButton* btn){
    buttons.Add(btn);
}

void UIManager::CursorMove(Vector2 pos){
    cursorPos = pos;
    if (leftDown && cur != NULL){
        cur->Drag(pos - startPos);
        return;
    }
    cur = NULL;
    buttons.Foreach<UIManager*>([](IButton* btn, UIManager* mgr){
        if(btn->Trigger(mgr->cursorPos)){
            mgr->cur = btn;
            mgr->startPos = mgr->cursorPos;
            btn->Hover();
        }else{
            btn->Leave();
        }
    }, this);
}

void UIManager::Render(float aspect){
    aspect = 1.0f / aspect;

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-aspect, aspect, -1.0, 1.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    buttons.Foreach([](IButton* btn){
        btn->Render();
    });
}

bool UIManager::LeftDown(){
    if (cur != NULL){
        leftDown = true;
        cur->Click();
        return true;
    }
    return false;
}

bool UIManager::LeftUp(){
    cur = NULL;
    if (leftDown){
        leftDown = false;
        return true;
    }
    return false;
}

IButton::IButton(){}
IButton::~IButton(){}
bool IButton::Trigger(Vector2 pos){ return false; }
void IButton::Hover(){}
void IButton::Click(){}
void IButton::Drag(Vector2 dir){}
void IButton::Leave(){}
void IButton::Render(){}

IOperation::IOperation(){}
IOperation::~IOperation(){}
void IOperation::OnEnter(){}
void IOperation::OnConfirm(){}
void IOperation::OnUndo(){}
void IOperation::OnRightDown(){}
void IOperation::OnRightUp(){}
void IOperation::OnMove(){}
void IOperation::OnCommand(UINT id){}

ViewportManager* ViewportManager::inst = new ViewportManager();

ViewportManager::ViewportManager(){}

ViewportManager::~ViewportManager(){}

void ViewportManager::Reset(HWND hWnd){
    RECT cliRect;
    LONG tmp;
    rects.Clear();
    GetClientRect(hWnd, &cliRect);
    tmp = cliRect.bottom; cliRect.bottom = cliRect.top; cliRect.top = tmp;
    rects.Add(cliRect);
    curRect = cliRect;
    glViewport(curRect.left, curRect.bottom, curRect.right - curRect.left, curRect.top - curRect.bottom);
    glScissor(curRect.left, curRect.bottom, curRect.right - curRect.left, curRect.top - curRect.bottom);
}

void ViewportManager::PushViewport(RECT rect){
    rects.Add(rect);
    curRect = rect;
    glViewport(curRect.left, curRect.bottom, curRect.right - curRect.left, curRect.top - curRect.bottom);
    glScissor(curRect.left, curRect.bottom, curRect.right - curRect.left, curRect.top - curRect.bottom);
}

void ViewportManager::PopViewport(){
    rects.RemoveBack();
    curRect = rects.GetBack();
    glViewport(curRect.left, curRect.bottom, curRect.right - curRect.left, curRect.top - curRect.bottom);
    glScissor(curRect.left, curRect.bottom, curRect.right - curRect.left, curRect.top - curRect.bottom);
}

RECT ViewportManager::GetCurrentRect(){
    return curRect;
}

LONG ViewportManager::GetCurrentWidth(){
    return curRect.right - curRect.left;
}

LONG ViewportManager::GetCurrentHeight(){
    return curRect.top - curRect.bottom;
}

float ViewportManager::GetAspect(){
    return (float)(curRect.right - curRect.left) / (curRect.top - curRect.bottom);
}

void ViewportManager::EnableScissor(){
    glEnable(GL_SCISSOR_TEST);
    glScissor(curRect.left, curRect.bottom, curRect.right - curRect.left, curRect.top - curRect.bottom);
}

void ViewportManager::DisableScissor(){
    glDisable(GL_SCISSOR_TEST);
}

Vector2 ViewportManager::GetClientSize(){
    return Vector2(curRect.right - curRect.left, curRect.top - curRect.bottom);
}

ITool::ITool(){}
ITool::~ITool(){}
void ITool::OnSelect(){}
void ITool::OnUnselect(){}
void ITool::OnLeftDown(){}
void ITool::OnLeftUp(){}
void ITool::OnRightDown(){}
void ITool::OnRightUp(){}
void ITool::OnMove(){}
void ITool::OnRender(){}

IWindow::IWindow(){}
IWindow::~IWindow(){}
void IWindow::SetFrame(HWND hWnd){}
LRESULT IWindow::OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){ return DefWindowProc(hWnd, uMsg, wParam, lParam); }
void IWindow::OnRender(){}
void IWindow::OnCreate(){}
void IWindow::OnClose(){}
void IWindow::OnResize(int x, int y){}
void IWindow::OnMouseMove(int x, int y){}
void IWindow::OnLeftDown(int x, int y){}
void IWindow::OnLeftUp(int x, int y){}
void IWindow::OnRightDown(int x, int y){}
void IWindow::OnRightUp(int x, int y){}
void IWindow::OnMouseHover(int key, int x, int y){}
void IWindow::OnMouseLeave(){}
void IWindow::OnFocus(HWND hLost){}
void IWindow::OnKillFocus(HWND hFocus){}
void IWindow::OnMouseWheel(int delta){}
void IWindow::OnMenuAccel(int id, bool accel){}
void IWindow::OnControl(int inform, int id, HWND hctl){}