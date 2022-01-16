#include "uimgr.h"

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

void UIManager::Render(){
    glDisable(GL_BLEND);
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