#include "uimgr.h"

#include <windows.h>

#include "opengl/gl/gl.h"

#include "font.h"
#include "main.h"

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
    glOrtho(-aspect, aspect, -1.0, 1.0, 0.0, 2.0);// OpenGL 深度[-1,1]，变换后比例不变
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
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 2.0);
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
    glOrtho(-aspect, aspect, -1.0, 1.0, 0.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void UIManager::RenderTransform(){
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool UIManager::LeftDown(){
    if (cur){
        leftDown = true;
        cur->Click(cursorPos);
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
void IButton::Click(Vector2 pos){}
void IButton::Drag(Vector2 dir){}
void IButton::ClickEnd(){}
void IButton::Leave(){}
bool IButton::Char(char c){ return false; }
bool IButton::Unichar(wchar_t c){ return false; }
void IButton::Render(){}

IconButton::IconButton(Vector2 position, Vector2 size) : position(position), size(size) {}
IconButton::IconButton(Vector2 position, Vector2 size, float radius) : position(position), size(size), radius(radius) {}

IconButton::~IconButton(){
    if (texture) delete texture;
}

bool IconButton::Trigger(Vector2 pos){
    return pos.x >= position.x && pos.x <= position.x + size.x && pos.y >= position.y && pos.y <= position.y + size.y;
}

void IconButton::Click(Vector2 pos){
    if (onClick)
        onClick(userData);
}

void IconButton::Render(){
    if (texture)
        texture->Enable();
    GLUtils::DrawRoundRectWithUV(
        GLRect(
            position.x, position.y,
            position.x + size.x, position.y + size.y
        ),
        radius, 0.05f
    );
    GLTexture2D::Disable();
}

void IconButton::OnClick(void(*func)(void*)){
    onClick = func;
}

void IconButton::SetUserData(void* data){
    userData = data;
}

void IconButton::SetIcon(const char* texPath){
    if (texture)
        delete texture;
    texture = new GLTexture2D(texPath);
}

void IconButton::SetIcon(int iconRes){
    if (texture)
        delete texture;
    texture = new GLTexture2D(iconRes);
}

void IconButton::SetIcon(GLTexture2D* tex){
    if (texture)
        delete texture;
    texture = tex;
}

UIEditA::UIEditA(Vector2 pos, float width) : position(pos), size(Vector2(width, 0.0f)) {
    text[0] = '\0';
}

UIEditA::UIEditA(Vector2 pos, float width, void(*onEdit)(char*, void*)) : position(pos), size(Vector2(width, 0.0f)), onEdit(onEdit) {
    text[0] = '\0';
}

UIEditA::UIEditA(Vector2 pos, float width, void(*onEdit)(char*, void*), void* userData) : position(pos), size(Vector2(width, 0.0f)), onEdit(onEdit), userData(userData) {
    text[0] = '\0';
}

UIEditA::~UIEditA(){}

bool UIEditA::Trigger(Vector2 pos){
    return pos.x >= position.x && pos.x <= position.x + size.x && pos.y >= position.y && pos.y <= position.y + size.y;
}

void UIEditA::Hover(){
    Main::SetWindowCursor(IDC_IBEAM);
}

void UIEditA::Click(Vector2 pos){
    Main::SetWindowCursor(IDC_IBEAM);
    editing = true;
}

void UIEditA::Leave(){
    if (editing){
        editing = false;
        text[editPos] = '\0';
        if (onEdit)
            onEdit(text, userData);
    }
}

bool UIEditA::Char(char c){
    if (!editing){
        return false;
    }
    switch (c){
    case '\r':
    case '\n':
        if (editing){
            editing = false;
            if (onEdit)
                onEdit(text, userData);
        }
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
    Vector2 cliSize = ViewportManager::inst->GetClientSize();
    Vector2 cliInvSize = Vector2(1.0f / cliSize.x, 1.0f / cliSize.y);
    float strWidth;

    size.y = cliInvSize.y * 30.0f;

    glEnable(GL_SCISSOR_TEST);
    ViewportManager::inst->SetChildScissor(position.x, position.x + size.x, position.y, position.y + size.y);

    glColor3f(bkColor.x, bkColor.y, bkColor.z);
    if (radius == 0.0f){
        GLUtils::DrawRect(position, position + size);
    }else{
        GLUtils::DrawRoundRect(GLRect(position, position + size), radius * size.y, 0.05f);
    }

    glColor3f(fontColor.x, fontColor.y, fontColor.z);
    glRasterPos2f(position.x, position.y + 6.0f * cliInvSize.y);
    glDrawCNString(text);

    if (editing){
        strWidth = glGetStringWidth(text);
        glColor3f(1.0f, 1.0f, 1.0f);
        GLUtils::DrawRect(position.x + (strWidth - 1.0f) * cliInvSize.x * 2.0f, position.y, position.x + (strWidth + 1.0f) * cliInvSize.x * 2.0f, position.y + size.y);
    }

    ViewportManager::inst->ResetScissor();
}

char* UIEditA::GetText(){ return text; }
size_t UIEditA::GetTextMaxLength(){ return MAX_PATH; }

void UIEditA::SetBackgroundColor(Vector3 color){ bkColor = color; }
void UIEditA::SetFontColor(Vector3 color){ fontColor = color; }
void UIEditA::SetSelectionColor(Vector3 color){ selColor = color; }
void UIEditA::SetCornerRadius(float radius){ this->radius = radius; }
Vector3 UIEditA::GetBackgroundColor(){ return bkColor; }
Vector3 UIEditA::GetFontColor(){ return fontColor; }
Vector3 UIEditA::GetSelectionColor(){ return selColor; }
float UIEditA::GetCornerRadius(){ return radius; }

UIEditW::UIEditW(Vector2 pos, float width) : position(pos), size(Vector2(width, 0.0f)) {
    text[0] = L'\0';
}

UIEditW::UIEditW(Vector2 pos, float width, void(*onEdit)(wchar_t*, void*)) : position(pos), size(Vector2(width, 0.0f)), onEdit(onEdit) {
    text[0] = L'\0';
}

UIEditW::UIEditW(Vector2 pos, float width, void(*onEdit)(wchar_t*, void*), void* userData) : position(pos), size(Vector2(width, 0.0f)), onEdit(onEdit), userData(userData) {
    text[0] = L'\0';
}

UIEditW::~UIEditW(){}

bool UIEditW::Trigger(Vector2 pos){
    return pos.x >= position.x && pos.x <= position.x + size.x && pos.y >= position.y && pos.y <= position.y + size.y;
}

void UIEditW::Hover(){
    Main::SetWindowCursor(IDC_IBEAM);
}

void UIEditW::Click(Vector2 pos){
    Main::SetWindowCursor(IDC_IBEAM);
    editing = true;
}

void UIEditW::Leave(){
    editing = false;
    text[editPos] = L'\0';
}

bool UIEditW::Unichar(wchar_t c){
    if (!editing){
        return false;
    }
    switch (c){
    case '\r':
    case '\n':
        if (editing){
            editing = false;
            if (onEdit)
                onEdit(text, userData);
        }
        return true;
    case L'\b':
        if (editPos == 0)
            return true;
        text[--editPos] = L'\0';
        return true;
    }
    if (editPos >= MAX_PATH){
        return true;
    }
    text[editPos++] = c;
    text[editPos] = L'\0';
    return true;
}

void UIEditW::Render(){
    Vector2 cliSize = ViewportManager::inst->GetClientSize();
    Vector2 cliInvSize = Vector2(1.0f / cliSize.x, 1.0f / cliSize.y);
    float strWidth;

    size.y = cliInvSize.y * 30.0f;

    glEnable(GL_SCISSOR_TEST);
    ViewportManager::inst->SetChildScissor(position.x, position.x + size.x, position.y, position.y + size.y);

    glColor3f(bkColor.x, bkColor.y, bkColor.z);
    if (radius == 0.0f){
        GLUtils::DrawRect(position, position + size);
    }else{
        GLUtils::DrawRoundRect(GLRect(position, position + size), radius * size.y, 0.05f);
    }

    glColor3f(fontColor.x, fontColor.y, fontColor.z);
    glRasterPos2f(position.x, position.y + 6.0f * cliInvSize.y);
    glDrawCNString(text);

    if (editing){
        strWidth = glGetCNStringWidth(text);
        glColor3f(1.0f, 1.0f, 1.0f);
        GLUtils::DrawRect(position.x + (strWidth - 1.0f) * cliInvSize.x * 2.0f, position.y, position.x + (strWidth + 1.0f) * cliInvSize.x * 2.0f, position.y + size.y);
    }

    ViewportManager::inst->ResetScissor();
}

wchar_t* UIEditW::GetText(){ return text; }
size_t UIEditW::GetTextMaxLength(){ return MAX_PATH; }

void UIEditW::SetBackgroundColor(Vector3 color){ bkColor = color; }
void UIEditW::SetFontColor(Vector3 color){ fontColor = color; }
void UIEditW::SetSelectionColor(Vector3 color){ selColor = color; }
void UIEditW::SetCornerRadius(float radius){ this->radius = radius; }
Vector3 UIEditW::GetBackgroundColor(){ return bkColor; }
Vector3 UIEditW::GetFontColor(){ return fontColor; }
Vector3 UIEditW::GetSelectionColor(){ return selColor; }
float UIEditW::GetCornerRadius(){ return radius; }

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

    left = (left + 1.0f) * 0.5f;
    right = (right + 1.0f) * 0.5f;
    bottom = (bottom + 1.0f) * 0.5f;
    top = (top + 1.0f) * 0.5f;

    height = GetCurrentHeight();
    width = GetCurrentWidth();

    rect.left = curRect.left + Round(width * Clamp(left, 0.0f, 1.0f));
    rect.right = curRect.left + Round(width * Clamp(right, 0.0f, 1.0f));
    rect.bottom = curRect.bottom + Round(height * Clamp(bottom, 0.0f, 1.0f));
    rect.top = curRect.bottom + Round(height * Clamp(top, 0.0f, 1.0f));

    return rect;
}

void ViewportManager::PushChildViewport(float left, float right, float bottom, float top){
    PushViewport(CalculateChildRect(left, right, bottom, top));
}

void ViewportManager::SetScissor(RECT rect){
    glScissor(rect.left, rect.bottom, rect.right - rect.left, rect.top - rect.bottom);
}

void ViewportManager::SetChildScissor(float left, float right, float bottom, float top){
    SetScissor(CalculateChildRect(left, right, bottom, top));
}

void ViewportManager::ResetScissor(){
    SetScissor(curRect);
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