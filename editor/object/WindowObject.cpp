#include <editor/object/WindowObject.h>

AWindowObject::AWindowObject() : AViewObject(L"Window", ViewObjectType::OBJECT_WINDOW) {}
AWindowObject::~AWindowObject(){}

void AWindowObject::UpdateCursor(Point2 pos){
    cursorPos = pos;
}

void AWindowObject::UpdateWindowSize(Vector2 size){
    cliSize = size;
    cliInvSize.x = 1.0f / cliSize.x;
    cliInvSize.y = 1.0f / cliSize.y;
    aspect = (float)cliSize.x / cliSize.y;
}

void AWindowObject::OnRender(){}
void AWindowObject::OnCreate(){}
void AWindowObject::OnClose(){}
void AWindowObject::OnTimer(int id){}
void AWindowObject::OnChar(char c){}
void AWindowObject::OnUnichar(wchar_t c){}

void AWindowObject::OnResize(Vector2 size){
    if (size.x == 0.0f || size.y == 0.0f){
        return;
    }
    UpdateWindowSize(size);
}

void AWindowObject::OnMouseMove2D(Point2 pos){
    UpdateCursor(pos);
}

void AWindowObject::OnLeftDown2D(Point2 pos){
    UpdateCursor(pos);
}

void AWindowObject::OnLeftUp2D(Point2 pos){
    UpdateCursor(pos);
}

void AWindowObject::OnRightDown2D(Point2 pos){
    UpdateCursor(pos);
}

void AWindowObject::OnRightUp2D(Point2 pos){
    UpdateCursor(pos);
}

void AWindowObject::OnFocus(){
    focus = true;
}

void AWindowObject::OnKillFocus(){
    focus = false;
}

void AWindowObject::OnMouseWheel(int delta){}
void AWindowObject::OnMenuAccel(int id, bool accel){}
void AWindowObject::OnDropFileA(const char* path){}
void AWindowObject::OnDropFileW(const wchar_t* path){}