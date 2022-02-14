#include "anim.h"

#include "main.h"
#include "font.h"
#include "timetools.h"

IAnimationFunction::IAnimationFunction(){}
IAnimationFunction::~IAnimationFunction(){}

float IAnimationFunction::GetValue(Vector2 p1, Vector2 p2, float val){ return p1.y; }

LinearFunc::LinearFunc(){}
LinearFunc::~LinearFunc(){}

float LinearFunc::GetValue(Vector2 p1, Vector2 p2, float val){
    return Lerp(p1.y, p2.y, GetRate(val, p1.x, p2.x));
}

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

void AnimationWindow::PlayButton::Click(Vector2 pos){
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

AnimationWindow::AnimationCurve::AnimationCurve(AnimationWindow* window) : window(window) {
    points.Add(Vector2(window->startFrame, 0.0f));
    points.Add(Vector2(window->endFrame, 0.0f));

    functions.Add(new IAnimationFunction());

    funcMenu = new Menu();
    funcMenu->AddItem(new MenuItem(L"添加点", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->functions.Insert(curve->selIndex + 1, new IAnimationFunction());
        curve->points.Insert(curve->selIndex + 1, Vector2(Lerp(curve->window->startFrame, curve->window->endFrame, (curve->window->cursorPos.x + 1.0f) * 0.5f), curve->window->cursorPos.y));
        curve->selTarget = NONE;
    }, this));
    funcMenu->AddItem(new MenuItem(L"删除点", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        if (curve->selIndex > 0){
            curve->points.RemoveAt(curve->selIndex);
            if (curve->functions[curve->selIndex]){
                delete curve->functions[curve->selIndex];
            }
            curve->functions.RemoveAt(curve->selIndex);
        }
        curve->selTarget = NONE;
    }, this));
    funcMenu->AddItem(new MenuItem());
    funcMenu->AddItem(new MenuItem(L"常量", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->SetFunc(curve->selIndex, new IAnimationFunction());
    }, this));
    funcMenu->AddItem(new MenuItem(L"线性", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->SetFunc(curve->selIndex, new LinearFunc());
    }, this));
}

AnimationWindow::AnimationCurve::~AnimationCurve(){
    Free(functions);
    if (funcMenu) delete funcMenu;
}

bool AnimationWindow::AnimationCurve::Trigger(Vector2 pos){
    return pos.y >= -0.9f && pos.y <= 0.8f;
}

void AnimationWindow::AnimationCurve::Click(Vector2 pos){
    size_t size = points.Size();
    size_t seg;
    float err = 400.0f * window->cliInvSize.y * window->cliInvSize.y;

    for (size_t i = 1; i < size - 1; i++){
        Vector2 pointPos = Vector2(GetRate(points[i].x, window->startFrame, window->endFrame) * 2.0f - 1.0f, points[i].y);
        if ((pointPos - pos).SqrMagnitude() <= err){
            selTarget = POINT;
            selIndex = i;
            initialPos = points[i];
            DebugLog("AnimationCurve::Click Select Point %d", selIndex);
            return;
        }
    }

    seg = GetSegment(Lerp(window->startFrame, window->endFrame, (pos.x + 1.0f) * 0.5f));
    if (seg != -1){
        if (selTarget == SEGMENT && selIndex == seg){
            if (TimeUtils::GetTime() - lastTime <= 0.5f){
                // 双击操作
                Main::SetMenu(funcMenu);
                return;
            }
            selTarget = NONE;
            return;
        }
        selTarget = SEGMENT;
        selIndex = seg;
        lastTime = TimeUtils::GetTime();
        DebugLog("AnimationCurve::Click Select Segment %d", selIndex);
        return;
    }

    selTarget = NONE;
    DebugLog("AnimationCurve::Click Select None");
}

void AnimationWindow::AnimationCurve::Drag(Vector2 dir){
    if (selTarget == POINT){
        points[selIndex] = initialPos + Vector2(dir.x * 0.5f * (window->endFrame - window->startFrame), dir.y);
    }
}

void AnimationWindow::AnimationCurve::Render(){
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(-1.0f, -0.8f, -1.0f);
    glVertex3f(1.0f, -0.8f, -1.0f);
    glVertex3f(1.0f, 0.9f, -1.0f);
    glVertex3f(-1.0f, 0.9f, -1.0f);
    glEnd();

    // 动画曲线绘制
    glEnable(GL_LINE_SMOOTH);
    if (selTarget == SEGMENT){
        glLineWidth(4.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (float pos = points[selIndex].x; pos <= points[selIndex + 1].x + 0.5f; pos += 1.0f){
            glVertex2f(GetRate(pos, window->startFrame, window->endFrame) * 2.0f - 1.0f, GetValue(pos));
        }
        glEnd();
    }
    glLineWidth(3.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (float pos = window->startFrame; pos <= window->endFrame + 0.5f; pos += 1.0f){
        glVertex2f(GetRate(pos, window->startFrame, window->endFrame) * 2.0f - 1.0f, GetValue(pos));
    }
    glEnd();
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);

    // 节点绘制
    glPointSize(4.0f);
    glEnable(GL_POINT_SMOOTH);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    points.Foreach<AnimationCurve*>([](Vector2 p, AnimationCurve* curve){
        glVertex2f(GetRate(p.x, curve->window->startFrame, curve->window->endFrame) * 2.0f - 1.0f, p.y);
    }, this);
    glEnd();
    if (selTarget == POINT){
        glPointSize(8.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertex2f(GetRate(points[selIndex].x, window->startFrame, window->endFrame) * 2.0f - 1.0f, points[selIndex].y);
        glEnd();
    }
    glDisable(GL_POINT_SMOOTH);
    glPointSize(1.0f);
}

size_t AnimationWindow::AnimationCurve::GetSegment(float pos){
    size_t size = points.Size();

    for (size_t i = 0; i < size - 1; i++){
        if (pos >= points[i].x && pos <= points[i + 1].x){
            return i;
        }
    }

    return -1;
}

float AnimationWindow::AnimationCurve::GetValue(float pos){
    size_t seg = GetSegment(pos);

    if (seg == -1){
        return 0.0f;
    }

    return functions[seg]->GetValue(points[seg], points[seg + 1], pos);
}

void AnimationWindow::AnimationCurve::OnChangeRange(float start, float end){
    points[0].x = start;
    points[points.Size() - 1].x = end;
}

void AnimationWindow::AnimationCurve::SetFunc(size_t seg, IAnimationFunction* func){
    if (functions[seg])
        delete functions[seg];
    functions[seg] = func;
}

AnimationWindow::AnimationWindow(){
    DebugLog("AnimationWindow Launched");

    uiMgr = new UIManager();

    curve = new AnimationCurve(this);

    uiMgr->AddButton(curve);
    uiMgr->AddButton(new Bottom());
    uiMgr->AddButton(new PlayButton(this));
    uiMgr->AddButton(new FrameIndicator(this));
}

AnimationWindow::~AnimationWindow(){
    DebugLog("AnimationWindow Destroyed");
    if (uiMgr) delete uiMgr;
}

bool AnimationWindow::IsFocus(){
    return focus;
}

void AnimationWindow::OnRender(){
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
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
        if (frame > endFrame){
            // 结束直接暂停
            //play = false;
            // 循环
            frame = startFrame;
        }
        frame = Clamp(frame, startFrame, endFrame);
    }
    Main::data->animFrame = frame;

    // 测试代码
    Main::data->curObject->transform.rotationMode = Transform::ROT_EULER_XYZ;
    Main::data->curObject->transform.rotationXYZ.x = 0.0f;
    Main::data->curObject->transform.rotationXYZ.y = curve->GetValue(frame) * 360.0f;
    Main::data->curObject->transform.rotationXYZ.z = 0.0f;
    Main::data->curObject->transform.position.x = 0.0f;
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