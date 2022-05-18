#include <editor/AnimationWindow.h>

#include <lib/opengl/gl/gl.h>

#include <main.h>
#include <res.h>
#include <util/os/Time.h>
#include <util/os/Font.h>
#include <util/gl/GLUtils.h>
#include <util/math3d/Math.h>
#include <util/math3d/Property.h>
#include <editor/main/ViewObject.h>
#include <editor/gui/AnimationCurve.h>

namespace simple3deditor {

class AnimationWindow::FrameIndicator : public IButton {
private:
    AnimationWindow* window;
    float pos = 0.0f;
    float origin;

public:
    FrameIndicator(AnimationWindow* window) : window(window) {
        pos = Clamp(((window->frame - window->startFrame) / (window->endFrame - window->startFrame)) * 2.0f - 1.0f, -1.0f, 1.0f);
    }

    virtual ~FrameIndicator() override{}

    virtual bool Trigger(Vector2 pos) override{
        return pos.x >= this->pos - 0.05f && pos.x <= this->pos + 0.05f && pos.y >= 0.9f && pos.y <= 1.0f;
    }

    virtual void Click(Vector2 pos) override{
        origin = this->pos;
    }

    virtual void Drag(Vector2 dir) override{
        pos = Clamp(origin + dir.x, -1.0f, 1.0f);
        window->SetFrame(Lerp(window->startFrame, window->endFrame, (pos + 1.0f) * 0.5f));
    }

    virtual void Render() override{
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
};

class AnimationWindow::Bottom : public IButton {
private:
    static constexpr Vector3 COLOR = {0.0f, 0.0f, 0.0f};
    static constexpr float DEPTH = -1.0f;
    static constexpr float BOUND_TOP = -0.8f;
    static constexpr float BOUND_BOTTOM = -1.0f;

public:
    Bottom(){}
    virtual ~Bottom() override{}

    virtual void Render() override{
        glColor3f(COLOR.x, COLOR.y, COLOR.z);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(-1.0f, BOUND_TOP, DEPTH);
        glVertex3f(1.0f, BOUND_TOP, DEPTH);
        glVertex3f(1.0f, BOUND_BOTTOM, DEPTH);
        glVertex3f(-1.0f, BOUND_BOTTOM, DEPTH);
        glEnd();
    }
};

class AnimationWindow::PlayButton : public IButton {
private:
    AnimationWindow* window;

public:
    PlayButton(AnimationWindow* window) : window(window) {}
    virtual ~PlayButton() override{}

    virtual bool Trigger(Vector2 pos) override{
        return pos.x >= -0.1f && pos.x <= 0.1f && pos.y >= -0.98f && pos.y <= -0.82f;
    }

    virtual void Click(Vector2 pos) override{
        window->IsPlaying() ? window->Stop() : window->Play();
        DebugLog("AnimationWindow %s", window->IsPlaying() ? "Play" : "Stop");
    }

    virtual void Render() override{
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
};

AnimationWindow::AnimationWindow(){
    DebugLog("AnimationWindow Launched");

    uiMgr = new UIManager();

    uiMgr->AddButton(new Bottom());
    uiMgr->AddButton(new PlayButton(this));
    uiMgr->AddButton(new FrameIndicator(this));

    basicMenu = new Menu();

    Menu* posMenu = new Menu();
    posMenu->AddItem(new MenuItem(L"X", [=]{ this->SetProperty(&Main::data->curObject->transform.position.x); }));
    posMenu->AddItem(new MenuItem(L"Y", [=]{ this->SetProperty(&Main::data->curObject->transform.position.y);}));
    posMenu->AddItem(new MenuItem(L"Z", [=]{ this->SetProperty(&Main::data->curObject->transform.position.z); }));
    basicMenu->AddItem(new MenuItem(L"位置", posMenu));

    Menu* quatMenu = new Menu();
    quatMenu->AddItem(new MenuItem(L"X", [=]{ this->SetProperty(&Main::data->curObject->transform.rotation.x); }));
    quatMenu->AddItem(new MenuItem(L"Y", [=]{ this->SetProperty(&Main::data->curObject->transform.rotation.y); }));
    quatMenu->AddItem(new MenuItem(L"Z", [=]{ this->SetProperty(&Main::data->curObject->transform.rotation.z); }));
    quatMenu->AddItem(new MenuItem(L"W", [=]{ this->SetProperty(&Main::data->curObject->transform.rotation.z); }));

    Menu* xyzMenu = new Menu();
    xyzMenu->AddItem(new MenuItem(L"X", [=]{ this->SetProperty(&Main::data->curObject->transform.rotationXYZ.x); }));
    xyzMenu->AddItem(new MenuItem(L"Y", [=]{ this->SetProperty(&Main::data->curObject->transform.rotationXYZ.y); }));
    xyzMenu->AddItem(new MenuItem(L"Z", [=]{ this->SetProperty(&Main::data->curObject->transform.rotationXYZ.z); }));

    MenuItem* rotMenu = new MenuItem();
    rotMenu->type = MenuItemType::GROUP;
    rotMenu->name = L"旋转";
    rotMenu->menu = [=]{
        if (Main::data->curObject->transform.rotationMode == Transform::ROT_QUATERNION)
            return quatMenu;
        return xyzMenu;
    };
    rotMenu->onDestory += [=](MenuItem* item){
        delete quatMenu;
        delete xyzMenu;
    };
    basicMenu->AddItem(rotMenu);

    Menu* scaleMenu = new Menu();
    scaleMenu->AddItem(new MenuItem(L"X", [=]{ this->SetProperty(&Main::data->curObject->transform.scale.x); }));
    scaleMenu->AddItem(new MenuItem(L"Y", [=]{ this->SetProperty(&Main::data->curObject->transform.scale.y); }));
    scaleMenu->AddItem(new MenuItem(L"Z", [=]{ this->SetProperty(&Main::data->curObject->transform.scale.z); }));
    basicMenu->AddItem(new MenuItem(L"尺寸", scaleMenu));

    Menu* rotModeMenu = new Menu();
    rotModeMenu->AddItem(new MenuItem(L"四元数", [=]{ Main::data->curObject->transform.rotationMode = Transform::ROT_QUATERNION; }));
    rotModeMenu->AddItem(new MenuItem(L"XYZ旋转", [=]{ Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_XYZ; }));
    rotModeMenu->AddItem(new MenuItem(L"XZY旋转", [=]{ Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_XZY; }));
    rotModeMenu->AddItem(new MenuItem(L"YZX旋转", [=]{ Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_YZX; }));
    rotModeMenu->AddItem(new MenuItem(L"YXZ旋转", [=]{ Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_YXZ; }));
    rotModeMenu->AddItem(new MenuItem(L"ZXY旋转", [=]{ Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_ZXY; }));
    rotModeMenu->AddItem(new MenuItem(L"ZYX旋转", [=]{ Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_ZYX; }));
    basicMenu->AddItem(new MenuItem(L"旋转模式", rotModeMenu));
    basicMenu->AddItem(new MenuItem());

    Menu* fpsMenu = new Menu();
    fpsMenu->AddItem(new MenuItem(L"10", [=]{ this->fps = 10.0f; }));
    fpsMenu->AddItem(new MenuItem(L"15", [=]{ this->fps = 15.0f; }));
    fpsMenu->AddItem(new MenuItem(L"20", [=]{ this->fps = 20.0f; }));
    fpsMenu->AddItem(new MenuItem(L"25", [=]{ this->fps = 25.0f; }));
    fpsMenu->AddItem(new MenuItem(L"30", [=]{ this->fps = 30.0f; }));
    fpsMenu->AddItem(new MenuItem(L"40", [=]{ this->fps = 40.0f; }));
    fpsMenu->AddItem(new MenuItem(L"50", [=]{ this->fps = 50.0f; }));
    fpsMenu->AddItem(new MenuItem(L"60", [=]{ this->fps = 60.0f; }));
    fpsMenu->AddItem(new MenuItem(L"75", [=]{ this->fps = 75.0f; }));
    fpsMenu->AddItem(new MenuItem(L"100", [=]{ this->fps = 100.0f; }));
    basicMenu->AddItem(new MenuItem(L"帧率", fpsMenu));

    Menu* highFpsMenu = new Menu();
    highFpsMenu->AddItem(new MenuItem(L"200", [=]{ this->fps = 200.0f; }));
    highFpsMenu->AddItem(new MenuItem(L"300", [=]{ this->fps = 300.0f; }));
    highFpsMenu->AddItem(new MenuItem(L"400", [=]{ this->fps = 400.0f; }));
    highFpsMenu->AddItem(new MenuItem(L"500", [=]{ this->fps = 500.0f; }));
    highFpsMenu->AddItem(new MenuItem(L"750", [=]{ this->fps = 750.0f; }));
    highFpsMenu->AddItem(new MenuItem(L"1000", [=]{ this->fps = 1000.0f; }));
    basicMenu->AddItem(new MenuItem(L"高帧率(仅供快放)", highFpsMenu));
}

AnimationWindow::~AnimationWindow(){
    DebugLog("AnimationWindow Destroyed");
    SetCurve(NULL);
    if (uiMgr) delete uiMgr;
    if (basicMenu) delete basicMenu;
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

    if (play){
        frame += Time::GetDeltaTime() * fps;
        if (frame > endFrame){
            // 结束直接暂停
            //play = false;
            //return;
            // 循环
            frame = startFrame;
        }
        frame = Clamp(frame, startFrame, endFrame);
        Main::OnAnimationFrame(frame);
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

void AnimationWindow::Serialize(IOutputStream& os){
    os.WriteWithLen(WINDOW_ID);
}

void AnimationWindow::Deserialize(IInputStream& os){}

void AnimationWindow::UpdateCursor(int x, int y){
    AWindow::UpdateCursor(x, y);
    uiMgr->CursorMove(cursorPos);
}

void AnimationWindow::UpdateWindowSize(int x, int y){
    AWindow::UpdateWindowSize(x, y);
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
}

void AnimationWindow::Stop(){
    play = false;
}

bool AnimationWindow::IsPlaying(){
    return play;
}

}