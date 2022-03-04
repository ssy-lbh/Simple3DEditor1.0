#include <editor/RenderWindow.h>

//TODO 待实现
RenderWindow::RenderWindow(){}

RenderWindow::~RenderWindow(){}

void RenderWindow::InitCamera(){

}

void RenderWindow::RenderModelView(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glDisable(GL_DITHER);
    glDisable(GL_FOG);
    glDisable(GL_LOGIC_OP);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool RenderWindow::IsFocus(){
    return focus;
}

void RenderWindow::OnRender(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderModelView();
}

void RenderWindow::OnClose(){}

void RenderWindow::OnTimer(int id){}

void RenderWindow::OnChar(char c){}

void RenderWindow::OnUnichar(wchar_t c){}

void RenderWindow::OnResize(int x, int y){
    UpdateWindowSize(x, y);
}

void RenderWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void RenderWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
}

void RenderWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
}

void RenderWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
}

void RenderWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
}

void RenderWindow::OnMouseHover(int key, int x, int y){}

void RenderWindow::OnMouseLeave(){}

void RenderWindow::OnFocus(){
    focus = true;
}

void RenderWindow::OnKillFocus(){
    focus = false;
}

void RenderWindow::OnMouseWheel(int delta){}

void RenderWindow::OnMenuAccel(int id, bool accel){}

void RenderWindow::OnDropFileA(const char* path){}

void RenderWindow::OnDropFileW(const wchar_t* path){}

void RenderWindow::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
}

void RenderWindow::UpdateCursor(int x, int y){
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 2.0f * y / cliSize.y - 1.0f;
}