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

void UIManager::Foreach(std::function<void(IButton*)> func){
    buttons.Foreach(func);
}

IButton* UIManager::GetCurrent(){
    return cur;
}

IButton* UIManager::FindCurrent(){
    cur = NULL;
    buttons.Foreach([=](IButton* btn){
        if(btn->Trigger(this->cursorPos)){
            this->cur = btn;
            this->startPos = this->cursorPos;
            btn->Hover(this->cursorPos);
        }else{
            btn->Leave(this->cursorPos);
        }
    });
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

IWindow::IWindow(){}
IWindow::~IWindow(){}
bool IWindow::IsFocus(){ return false; }
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
void IWindow::OnFocus(){}
void IWindow::OnKillFocus(){}
void IWindow::OnMouseWheel(int delta){}
void IWindow::OnMenuAccel(int id, bool accel){}
void IWindow::OnDropFileA(const char* path){}
void IWindow::OnDropFileW(const wchar_t* path){}