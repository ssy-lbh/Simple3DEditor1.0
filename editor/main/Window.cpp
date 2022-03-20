#include <editor/main/Window.h>

AWindow::AWindow(){}
AWindow::~AWindow(){}

void AWindow::UpdateCursor(int x, int y){
    cursorCoord.x = x;
    cursorCoord.y = y;
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 2.0f * y / cliSize.y - 1.0f;
}

void AWindow::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
    cliInvSize.x = 1.0f / cliSize.x;
    cliInvSize.y = 1.0f / cliSize.y;
    aspect = (float)cliSize.x / cliSize.y;
}

bool AWindow::IsFocus(){
    return focus;
}

void AWindow::OnRender(){}
void AWindow::OnCreate(){}
void AWindow::OnClose(){}
void AWindow::OnTimer(int id){}
void AWindow::OnChar(char c){}
void AWindow::OnUnichar(wchar_t c){}

void AWindow::OnResize(int x, int y){
    if (x == 0 || y == 0){
        return;
    }
    cliSize.x = x;
    cliSize.y = y;
    cliInvSize.x = 1.0f / cliSize.x;
    cliInvSize.y = 1.0f / cliSize.y;
    aspect = (float)cliSize.x / cliSize.y;
}

void AWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void AWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
}

void AWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
}

void AWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
}

void AWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
}

void AWindow::OnFocus(){
    focus = true;
}

void AWindow::OnKillFocus(){
    focus = false;
}

void AWindow::OnMouseWheel(int delta){}
void AWindow::OnMenuAccel(int id, bool accel){}
void AWindow::OnDropFileA(const char* path){}
void AWindow::OnDropFileW(const wchar_t* path){}