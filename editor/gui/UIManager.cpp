#include <editor/gui/UIManager.h>

#include <lib/opengl/gl/gl.h>

#include <main.h>
#include <editor/gui/ViewManager.h>
#include <utils/gl/GLUtils.h>
#include <utils/gl/GLTexture2D.h>
#include <utils/os/Font.h>

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
    if (rightDown && cur != NULL){
        cur->RightDrag(pos - startPos);
        return;
    }
    FindCurrent();
}

void UIManager::Render(){
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    buttons.Foreach([](IButton* btn){
        btn->Render();
    });
}

void UIManager::RenderWithDepth(){
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    buttons.Foreach([](IButton* btn){
        btn->Render();
    });
}

bool UIManager::LeftDown(){
    if (focus != cur){
        if (focus)
            focus->OnKillFocus(cursorPos, focus);
        focus = cur;
        if (focus)
            focus->OnFocus(cursorPos);
    }
    if (cur){
        leftDown = true;
        cur->Click(cursorPos);
        return true;
    }
    return false;
}

bool UIManager::LeftUp(){
    if (cur){
        IButton* preBtn = cur;
        FindCurrent();
        preBtn->ClickEnd(cursorPos, cur);
    }
    if (leftDown){
        leftDown = false;
        return true;
    }
    return false;
}

bool UIManager::RightDown(){
    if (focus != cur){
        if (focus)
            focus->OnKillFocus(cursorPos, focus);
        focus = cur;
        if (focus)
            focus->OnFocus(cursorPos);
    }
    if (cur){
        rightDown = true;
        cur->RightClick(cursorPos);
        return true;
    }
    return false;
}

bool UIManager::RightUp(){
    if (cur){
        IButton* preBtn = cur;
        FindCurrent();
        preBtn->RightClickEnd(cursorPos, cur);
    }
    if (rightDown){
        rightDown = false;
        return true;
    }
    return false;
}

bool UIManager::Wheel(int delta){
    if (cur){
        cur->OnWheel(delta);
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

IButton* UIManager::GetCurrent(){
    return cur;
}

IButton* UIManager::FindCurrent(){
    cur = NULL;
    buttons.Foreach<UIManager*>([](IButton* btn, UIManager* mgr){
        if(btn->Trigger(mgr->cursorPos)){
            mgr->cur = btn;
            mgr->startPos = mgr->cursorPos;
            btn->Hover(mgr->cursorPos);
        }else{
            btn->Leave(mgr->cursorPos);
        }
    }, this);
    return cur;
}

IButton::IButton(){}
IButton::~IButton(){}
bool IButton::Trigger(Vector2 pos){ return false; }
void IButton::Hover(Vector2 pos){}
void IButton::Click(Vector2 pos){}
void IButton::Drag(Vector2 dir){}
void IButton::ClickEnd(Vector2 pos, IButton* end){}
void IButton::Leave(Vector2 pos){}
void IButton::OnFocus(Vector2 pos){}
void IButton::OnKillFocus(Vector2 pos, IButton* focus){}
void IButton::RightClick(Vector2 pos){}
void IButton::RightDrag(Vector2 dir){}
void IButton::RightClickEnd(Vector2 pos, IButton* end){}
void IButton::OnWheel(int delta){}
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

void UIEditA::Hover(Vector2 pos){
    Main::SetCursor(IDC_IBEAM);
}

void UIEditA::Click(Vector2 pos){
    Main::SetCursor(IDC_IBEAM);
    editing = true;
}

void UIEditA::Leave(Vector2 pos){
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
    if (editPos >= DEFAULT_STRING_LENGTH){
        return true;
    }
    text[editPos++] = c;
    text[editPos] = '\0';
    return true;
}

void UIEditA::Render(){
    ViewManager* viewMgr = ViewManager::GetLocalInst();

    Vector2 cliSize = viewMgr->GetSize();
    Vector2 cliInvSize = Vector2(1.0f / cliSize.x, 1.0f / cliSize.y);
    float strWidth;

    size.y = cliInvSize.y * 30.0f;

    viewMgr->EnableScissor();
    viewMgr->SetChildScissor(GLRect(position, position + size));

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

    viewMgr->ResetScissor();
}

char* UIEditA::GetText(){ return text; }
size_t UIEditA::GetTextMaxLength(){ return DEFAULT_STRING_LENGTH; }

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

void UIEditW::Hover(Vector2 pos){
    Main::SetCursor(IDC_IBEAM);
}

void UIEditW::Click(Vector2 pos){
    Main::SetCursor(IDC_IBEAM);
    editing = true;
}

void UIEditW::Leave(Vector2 pos){
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
    if (editPos >= DEFAULT_STRING_LENGTH){
        return true;
    }
    text[editPos++] = c;
    text[editPos] = L'\0';
    return true;
}

void UIEditW::Render(){
    ViewManager* viewMgr = ViewManager::GetLocalInst();

    Vector2 cliSize = viewMgr->GetSize();
    Vector2 cliInvSize = Vector2(1.0f / cliSize.x, 1.0f / cliSize.y);
    float strWidth;

    size.y = cliInvSize.y * 30.0f;

    viewMgr->EnableScissor();
    viewMgr->SetChildScissor(GLRect(position, position + size));

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

    viewMgr->ResetScissor();
}

wchar_t* UIEditW::GetText(){ return text; }
size_t UIEditW::GetTextMaxLength(){ return DEFAULT_STRING_LENGTH; }

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