#include "uimgr.h"

#include <windows.h>
#include <windowsx.h>

#include "opengl/gl/gl.h"

#include "font.h"

UIManager::UIManager(){}

UIManager::~UIManager(){
    Free(buttons);
}

void UIManager::AddButton(IButton* btn){
    buttons.Add(btn);
}

void UIManager::DeleteButton(IButton* btn){
    if (cur == btn){
        cur = NULL;
    }
    if (!buttons.Remove(btn)){
        DebugLog("UIManager::DeleteButton Failed");
    }
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

void UIManager::Render(){
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    buttons.Foreach([](IButton* btn){
        btn->Render();
    });
}

void UIManager::RenderRaw(){
    buttons.Foreach([](IButton* btn){
        btn->Render();
    });
}

void UIManager::RenderTransform(float aspect){
    aspect = 1.0f / aspect;

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-aspect, aspect, -1.0, 1.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void UIManager::RenderTransform(){
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool UIManager::LeftDown(){
    if (cur){
        leftDown = true;
        cur->Click();
        return true;
    }
    return false;
}

bool UIManager::LeftUp(){
    if (cur){
        cur->ClickEnd();
        cur = NULL;
    }
    if (leftDown){
        leftDown = false;
        return true;
    }
    return false;
}

bool UIManager::Char(char c){
    if (cur){
        return cur->Char(c);
    }
    return false;
}

bool UIManager::Unichar(wchar_t c){
    if (cur){
        return cur->Unichar(c);
    }
    return false;
}

void UIManager::Foreach(void(*func)(IButton*)){
    buttons.Foreach(func);
}

void UIManager::Foreach(void(*func)(IButton*, void*), void* user){
    buttons.Foreach(func, user);
}

IButton::IButton(){}
IButton::~IButton(){}
bool IButton::Trigger(Vector2 pos){ return false; }
void IButton::Hover(){}
void IButton::Click(){}
void IButton::Drag(Vector2 dir){}
void IButton::ClickEnd(){}
void IButton::Leave(){}
bool IButton::Char(char c){ return false; }
bool IButton::Unichar(wchar_t c){ return false; }
void IButton::Render(){}

UIEditA::UIEditA(Vector2 pos, Vector2 size) : position(pos), size(size) {
    text[0] = '\0';
}

UIEditA::~UIEditA(){}

bool UIEditA::Trigger(Vector2 pos){
    return pos.x >= position.x && pos.x <= position.x + size.x && pos.y >= position.y && pos.y <= position.y + size.y;
}

void UIEditA::Hover(){
    SetCursor(LoadCursor(NULL, IDC_IBEAM));
}

void UIEditA::Click(){
    editing = true;
    editPos = 0;
}

void UIEditA::Leave(){
    editing = false;
    text[editPos] = '\0';
}

bool UIEditA::Char(char c){
    if (!editing){
        return false;
    }
    switch (c){
    case '\r':
        editing = false;
        return true;
    case '\n':
        return true;
    case '\b':
        if (editPos == 0)
            return true;
        text[--editPos] = '\0';
        return true;
    }
    if (editPos >= MAX_PATH){
        return true;
    }
    text[editPos++] = c;
    text[editPos] = '\0';
    return true;
}

void UIEditA::Render(){
    glColor3f(bkColor.x, bkColor.y, bkColor.z);
    GLUtils::DrawRect(position, position + size);

    //TODO 补全图形功能

    glColor3f(fontColor.x, fontColor.y, fontColor.z);
    glRasterPos2f(position.x, position.y);
    glDrawString(text);
}

void UIEditA::SetBackgroundColor(Vector3 color){ bkColor = color; }
void UIEditA::SetFontColor(Vector3 color){ fontColor = color; }
void UIEditA::SetSelectionColor(Vector3 color){ selColor = color; }
Vector3 UIEditA::GetBackgroundColor(){ return bkColor; }
Vector3 UIEditA::GetFontColor(){ return fontColor; }
Vector3 UIEditA::GetSelectionColor(){ return selColor; }

IOperation::IOperation(){}
IOperation::~IOperation(){}
void IOperation::OnEnter(){}
void IOperation::OnConfirm(){}
void IOperation::OnUndo(){}
void IOperation::OnRightDown(){}
void IOperation::OnRightUp(){}
void IOperation::OnMove(){}
void IOperation::OnCommand(int id){}

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

void ViewportManager::SetViewport(RECT rect){
    curRect = rect;
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

RECT ViewportManager::CalculateChildRect(float left, float right, float bottom, float top){
    RECT rect;
    LONG height, width;

    height = GetCurrentHeight();
    width = GetCurrentWidth();

    rect.left = curRect.left + __builtin_roundf(width * Clamp(left, 0.0f, 1.0f));
    rect.right = curRect.left + __builtin_roundf(width * Clamp(right, 0.0f, 1.0f));
    rect.bottom = curRect.bottom + __builtin_roundf(height * Clamp(bottom, 0.0f, 1.0f));
    rect.top = curRect.bottom + __builtin_roundf(height * Clamp(top, 0.0f, 1.0f));

    return rect;
}

void ViewportManager::PushChildViewport(float left, float right, float bottom, float top){
    PushViewport(CalculateChildRect(left, right, bottom, top));
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
void ITool::OnCommand(int id){}

IWindow::IWindow(){}
IWindow::~IWindow(){}
bool IWindow::IsFocus(){ return true; }
void IWindow::OnRender(){}
void IWindow::OnCreate(){}
void IWindow::OnClose(){}
void IWindow::OnTimer(int id){}
void IWindow::OnChar(char c){}
void IWindow::OnUnichar(wchar_t c){}
void IWindow::OnResize(int x, int y){}
void IWindow::OnMouseMove(int x, int y){}
void IWindow::OnLeftDown(int x, int y){}
void IWindow::OnLeftUp(int x, int y){}
void IWindow::OnRightDown(int x, int y){}
void IWindow::OnRightUp(int x, int y){}
void IWindow::OnMouseHover(int key, int x, int y){}
void IWindow::OnMouseLeave(){}
void IWindow::OnFocus(){}
void IWindow::OnKillFocus(){}
void IWindow::OnMouseWheel(int delta){}
void IWindow::OnMenuAccel(int id, bool accel){}
void IWindow::OnDropFileA(const char* path){}
void IWindow::OnDropFileW(const wchar_t* path){}