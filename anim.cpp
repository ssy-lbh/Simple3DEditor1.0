#include "anim.h"

#include "main.h"
#include "font.h"

AnimationWindow::FrameIndicator::FrameIndicator(AnimationWindow* window) : window(window) {
    pos = Clamp(((window->frame - window->startFrame) / (window->endFrame - window->startFrame)) * 2.0f - 1.0f, -1.0f, 1.0f);
}

AnimationWindow::FrameIndicator::~FrameIndicator(){}

bool AnimationWindow::FrameIndicator::Trigger(Vector2 pos){
    return pos.x >= this->pos - 0.05f && pos.x <= this->pos + 0.05f && pos.y >= 0.9f && pos.y <= 1.0f;
}

void AnimationWindow::FrameIndicator::Click(){
    origin = pos;
}

void AnimationWindow::FrameIndicator::Drag(Vector2 dir){
    pos = Clamp(origin + dir.x, -1.0f, 1.0f);
    window->frame = Lerp(window->startFrame, window->endFrame, (pos + 1.0f) * 0.5f);
}

void AnimationWindow::FrameIndicator::Render(){
    char frame[10];
    int width;

    pos = Clamp(((window->frame - window->startFrame) / (window->endFrame - window->startFrame)) * 2.0f - 1.0f, -1.0f, 1.0f);

    glLineWidth(2.0f);
    glColor3f(0.2f, 0.3f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(pos, 1.0f);
    glVertex2f(pos, -0.8f);
    glEnd();
    glLineWidth(1.0f);

    glColor3f(0.2f, 0.3f, 1.0f);
    GLUtils::DrawRoundRect(pos - 0.05f, 0.9f, 0.1f, 0.1f, 0.02f, 0.05f);

    __builtin_snprintf(frame, 10, "%d", (int)__builtin_floor(window->frame));
    width = glGetStringWidth(frame);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(pos - width * window->cliInvSize.x, 0.95f - 6.0f * window->cliInvSize.y);
    glDrawString(frame);
}

AnimationWindow::Bottom::Bottom(){}
AnimationWindow::Bottom::~Bottom(){}

void AnimationWindow::Bottom::Render(){
    glLineWidth(1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(-1.0f, -0.8f, -1.0f);
    glVertex3f(1.0f, -0.8f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
}

AnimationWindow::PlayButton::PlayButton(AnimationWindow* window) : window(window) {}
AnimationWindow::PlayButton::~PlayButton(){}

bool AnimationWindow::PlayButton::Trigger(Vector2 pos){
    return pos.x >= -0.1f && pos.x <= 0.1f && pos.y >= -0.98f && pos.y <= -0.82f;
}

void AnimationWindow::PlayButton::Click(){
    window->play = !window->play;
    if (window->frame == window->endFrame)
        window->frame = window->startFrame;
}

void AnimationWindow::PlayButton::Render(){
    glColor3f(0.3f, 0.3f, 0.3f);
    GLUtils::DrawRect(-0.1f, -0.98f, 0.1f, -0.82f);

    if (!window->play){
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.05f, -0.97f);
        glVertex2f(0.05f, -0.9f);
        glVertex2f(-0.05f, -0.83f);
        glEnd();
    }else{
        glColor3f(1.0f, 1.0f, 1.0f);
        GLUtils::DrawRect(-0.04f, -0.97f, -0.02f, -0.83f);
        GLUtils::DrawRect(0.02f, -0.97f, 0.04f, -0.83f);
    }
}

AnimationWindow::AnimationWindow(){
    uiMgr = new UIManager();

    uiMgr->AddButton(new FrameIndicator(this));
    uiMgr->AddButton(new Bottom());
    uiMgr->AddButton(new PlayButton(this));
}

AnimationWindow::~AnimationWindow(){
    if (uiMgr) delete uiMgr;
}

bool AnimationWindow::IsFocus(){
    return focus;
}

void AnimationWindow::OnRender(){
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    uiMgr->Render();
}

void AnimationWindow::OnTimer(int id){
    if (play){
        frame += 0.01f * fps;
        Main::RequestRender();
        if (frame > endFrame)
            play = false;
        frame = Clamp(frame, startFrame, endFrame);
    }
    Main::data->animFrame = frame;
}

void AnimationWindow::OnResize(int x, int y){
    UpdateWindowSize(x, y);
}

void AnimationWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void AnimationWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    uiMgr->LeftDown();
}

void AnimationWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
    uiMgr->LeftUp();
}

void AnimationWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
}

void AnimationWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
}

void AnimationWindow::OnFocus(){
    focus = true;
}

void AnimationWindow::OnKillFocus(){
    focus = false;
}

void AnimationWindow::OnMouseWheel(int delta){}
void AnimationWindow::OnMenuAccel(int id, bool accel){}

void AnimationWindow::UpdateCursor(int x, int y){
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 2.0f * y / cliSize.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
}

void AnimationWindow::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
    cliInvSize.x = 1.0f / cliSize.x;
    cliInvSize.y = 1.0f / cliSize.y;
    aspect = (float)cliSize.x / cliSize.y;
}