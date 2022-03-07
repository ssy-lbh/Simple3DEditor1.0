#include <editor/AnimationWindow.h>

#include <lib/opengl/gl/gl.h>

#include <main.h>
#include <res.h>
#include <utils/os/Time.h>
#include <utils/os/Font.h>
#include <utils/gl/GLUtils.h>
#include <utils/math3d/Math.h>
#include <utils/math3d/ViewObject.h>
#include <utils/math3d/Property.h>
#include <editor/gui/AnimationCurve.h>

AnimationWindow::FrameIndicator::FrameIndicator(AnimationWindow* window) : window(window) {
    pos = Clamp(((window->frame - window->startFrame) / (window->endFrame - window->startFrame)) * 2.0f - 1.0f, -1.0f, 1.0f);
}

AnimationWindow::FrameIndicator::~FrameIndicator(){}

bool AnimationWindow::FrameIndicator::Trigger(Vector2 pos){
    return pos.x >= this->pos - 0.05f && pos.x <= this->pos + 0.05f && pos.y >= 0.9f && pos.y <= 1.0f;
}

void AnimationWindow::FrameIndicator::Click(Vector2 pos){
    origin = this->pos;
}

void AnimationWindow::FrameIndicator::Drag(Vector2 dir){
    pos = Clamp(origin + dir.x, -1.0f, 1.0f);
    window->SetFrame(Lerp(window->startFrame, window->endFrame, (pos + 1.0f) * 0.5f));
}

void AnimationWindow::FrameIndicator::Render(){
    char frame[10];
    float width;

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

    __builtin_snprintf(frame, 10, "%d", (int)Floor(window->frame));
    width = glGetStringWidth(frame);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(pos - width * window->cliInvSize.x, 0.95f - 6.0f * window->cliInvSize.y);
    glDrawString(frame);
}

const Vector3 AnimationWindow::Bottom::COLOR = Vector3(0.0f, 0.0f, 0.0f);
const float AnimationWindow::Bottom::DEPTH = -1.0f;
const float AnimationWindow::Bottom::BOUND_TOP = -0.8f;
const float AnimationWindow::Bottom::BOUND_BOTTOM = -1.0f;

AnimationWindow::Bottom::Bottom(){}
AnimationWindow::Bottom::~Bottom(){}

void AnimationWindow::Bottom::Render(){
    glColor3f(COLOR.x, COLOR.y, COLOR.z);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(-1.0f, BOUND_TOP, DEPTH);
    glVertex3f(1.0f, BOUND_TOP, DEPTH);
    glVertex3f(1.0f, BOUND_BOTTOM, DEPTH);
    glVertex3f(-1.0f, BOUND_BOTTOM, DEPTH);
    glEnd();
}

AnimationWindow::PlayButton::PlayButton(AnimationWindow* window) : window(window) {}
AnimationWindow::PlayButton::~PlayButton(){}

bool AnimationWindow::PlayButton::Trigger(Vector2 pos){
    return pos.x >= -0.1f && pos.x <= 0.1f && pos.y >= -0.98f && pos.y <= -0.82f;
}

void AnimationWindow::PlayButton::Click(Vector2 pos){
    window->IsPlaying() ? window->Stop() : window->Play();
    DebugLog("AnimationWindow %s", window->IsPlaying() ? "Play" : "Stop");
}

void AnimationWindow::PlayButton::Render(){
    glColor3f(0.3f, 0.3f, 0.3f);
    GLUtils::DrawRect(-0.1f, -0.98f, 0.1f, -0.82f);

    if (!window->IsPlaying()){
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

AnimationWindow::RotationMenu::RotationMenu(AnimationWindow* window) : window(window) {
    quatMenu = new Menu();
    quatMenu->AddItem(new MenuItem(L"X", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.rotation.x);
    }, window));
    quatMenu->AddItem(new MenuItem(L"Y", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.rotation.y);
    }, window));
    quatMenu->AddItem(new MenuItem(L"Z", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.rotation.z);
    }, window));
    quatMenu->AddItem(new MenuItem(L"W", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.rotation.z);
    }, window));

    xyzMenu = new Menu();
    xyzMenu->AddItem(new MenuItem(L"X", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.rotationXYZ.x);
    }, window));
    xyzMenu->AddItem(new MenuItem(L"Y", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.rotationXYZ.y);
    }, window));
    xyzMenu->AddItem(new MenuItem(L"Z", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.rotationXYZ.z);
    }, window));
}

AnimationWindow::RotationMenu::~RotationMenu(){
    delete quatMenu;
    delete xyzMenu;
}

IMenuItem::ItemType AnimationWindow::RotationMenu::GetType(){
    return ItemType::GROUP;
}

const wchar_t* AnimationWindow::RotationMenu::GetName(){
    return L"旋转";
}

Menu* AnimationWindow::RotationMenu::GetMenu(){
    if (Main::data->curObject->transform.rotationMode == Transform::ROT_QUATERNION)
        return quatMenu;
    return xyzMenu;
}

const float AnimationWindow::DEFAULT_START_FRAME = 0.0f;
const float AnimationWindow::DEFAULT_END_FRAME = 250.0f;

AnimationWindow::AnimationWindow(){
    DebugLog("AnimationWindow Launched");

    uiMgr = new UIManager();

    uiMgr->AddButton(new Bottom());
    uiMgr->AddButton(new PlayButton(this));
    uiMgr->AddButton(new FrameIndicator(this));

    basicMenu = new Menu();

    Menu* posMenu = new Menu();
    posMenu->AddItem(new MenuItem(L"X", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.position.x);
    }, this));
    posMenu->AddItem(new MenuItem(L"Y", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.position.y);
    }, this));
    posMenu->AddItem(new MenuItem(L"Z", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.position.z);
    }, this));
    basicMenu->AddItem(new MenuItem(L"位置", posMenu));

    basicMenu->AddItem(new RotationMenu(this));

    Menu* scaleMenu = new Menu();
    scaleMenu->AddItem(new MenuItem(L"X", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.scale.x);
    }, this));
    scaleMenu->AddItem(new MenuItem(L"Y", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.scale.y);
    }, this));
    scaleMenu->AddItem(new MenuItem(L"Z", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->SetProperty(&Main::data->curObject->transform.scale.z);
    }, this));
    basicMenu->AddItem(new MenuItem(L"尺寸", scaleMenu));

    Menu* rotModeMenu = new Menu();
    rotModeMenu->AddItem(new MenuItem(L"四元数", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        Main::data->curObject->transform.rotationMode = Transform::ROT_QUATERNION;
    }, this));
    rotModeMenu->AddItem(new MenuItem(L"XYZ旋转", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_XYZ;
    }, this));
    rotModeMenu->AddItem(new MenuItem(L"XZY旋转", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_XZY;
    }, this));
    rotModeMenu->AddItem(new MenuItem(L"YZX旋转", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_YZX;
    }, this));
    rotModeMenu->AddItem(new MenuItem(L"YXZ旋转", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_YXZ;
    }, this));
    rotModeMenu->AddItem(new MenuItem(L"ZXY旋转", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_ZXY;
    }, this));
    rotModeMenu->AddItem(new MenuItem(L"ZYX旋转", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_ZYX;
    }, this));
    basicMenu->AddItem(new MenuItem(L"旋转模式", rotModeMenu));
    basicMenu->AddItem(new MenuItem());

    Menu* fpsMenu = new Menu();
    fpsMenu->AddItem(new MenuItem(L"10", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 10.0f;
    }, this));
    fpsMenu->AddItem(new MenuItem(L"15", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 15.0f;
    }, this));
    fpsMenu->AddItem(new MenuItem(L"20", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 20.0f;
    }, this));
    fpsMenu->AddItem(new MenuItem(L"25", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 25.0f;
    }, this));
    fpsMenu->AddItem(new MenuItem(L"30", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 30.0f;
    }, this));
    fpsMenu->AddItem(new MenuItem(L"40", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 40.0f;
    }, this));
    fpsMenu->AddItem(new MenuItem(L"50", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 50.0f;
    }, this));
    fpsMenu->AddItem(new MenuItem(L"60", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 60.0f;
    }, this));
    fpsMenu->AddItem(new MenuItem(L"75", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 75.0f;
    }, this));
    fpsMenu->AddItem(new MenuItem(L"100", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 100.0f;
    }, this));
    basicMenu->AddItem(new MenuItem(L"帧率", fpsMenu));

    Menu* highFpsMenu = new Menu();
    highFpsMenu->AddItem(new MenuItem(L"200", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 200.0f;
    }, this));
    highFpsMenu->AddItem(new MenuItem(L"300", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 300.0f;
    }, this));
    highFpsMenu->AddItem(new MenuItem(L"400", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 400.0f;
    }, this));
    highFpsMenu->AddItem(new MenuItem(L"500", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 500.0f;
    }, this));
    highFpsMenu->AddItem(new MenuItem(L"750", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 750.0f;
    }, this));
    highFpsMenu->AddItem(new MenuItem(L"1000", MENUITEM_LAMBDA_TRANS(AnimationWindow)[](AnimationWindow* window){
        window->fps = 1000.0f;
    }, this));
    basicMenu->AddItem(new MenuItem(L"高帧率(仅供快放)", highFpsMenu));
}

AnimationWindow::~AnimationWindow(){
    DebugLog("AnimationWindow Destroyed");
    SetCurve(NULL);
    if (uiMgr) delete uiMgr;
    if (basicMenu) delete basicMenu;
}

bool AnimationWindow::IsFocus(){
    return focus;
}

void AnimationWindow::OnRender(){
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLUtils::ResetProjection();
    GLUtils::ResetModelView();

    uiMgr->RenderWithDepth();

    glDepthFunc(GL_LESS);
}

void AnimationWindow::OnTimer(int id){
    switch (id){
    // 标准10ms时钟
    case 1:
        if (play){
            float curTime = TimeUtils::GetTime();

            frame += (curTime - time) * fps;
            time = curTime;
            if (frame > endFrame){
                // 结束直接暂停
                //play = false;
                //break;
                // 循环
                frame = startFrame;
            }
            frame = Clamp(frame, startFrame, endFrame);
            Main::OnAnimationFrame(frame);
        }
        break;
    }
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
    Main::SetMenu(basicMenu);
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

void AnimationWindow::OnMouseWheel(int delta){
    uiMgr->Wheel(delta);
}

void AnimationWindow::OnMenuAccel(int id, bool accel){
    switch (id){
    case IDM_MENU_BASIC:
        Main::SetMenu(basicMenu);
        break;
    }
}

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

void AnimationWindow::SetCurve(AnimationCurve* curve){
    if (this->curve)
        uiMgr->DeleteButton(this->curve);
    this->curve = curve;
    if (curve){
        uiMgr->AddButton(curve);
        curve->FlushRange();
    }
}

void AnimationWindow::SetProperty(Property* prop){
    if (!prop){
        SetCurve(NULL);
        return;
    }
    if (!prop->GetCurve()){
        SetCurve(new AnimationCurve(startFrame, endFrame));
        prop->SetCurve(curve);
    }else{
        SetCurve(prop->GetCurve());
        curve->OnChangeRange(startFrame, endFrame);
    }
}

void AnimationWindow::SetFrame(float frame){
    this->frame = Clamp(frame, startFrame, endFrame);
    Main::OnAnimationFrame(this->frame);
}

void AnimationWindow::Play(){
    play = true;
    time = TimeUtils::GetTime();
}

void AnimationWindow::Stop(){
    play = false;
}

bool AnimationWindow::IsPlaying(){
    return play;
}