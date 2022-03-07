#include <editor/gui/AnimationCurve.h>

#include <lib/opengl/gl/gl.h>

#include <main.h>
#include <editor/gui/Menu.h>
#include <editor/gui/ViewManager.h>
#include <utils/os/Font.h>
#include <utils/math3d/Math.h>

IAnimationFunction::IAnimationFunction(){}
IAnimationFunction::~IAnimationFunction(){}

float IAnimationFunction::GetValue(Vector2 p1, Vector2 p2, float val){ return p1.y; }

RightValueFunc::RightValueFunc(){}
RightValueFunc::~RightValueFunc(){}

float RightValueFunc::GetValue(Vector2 p1, Vector2 p2, float val){ return p2.y; }

LinearFunc::LinearFunc(){}
LinearFunc::~LinearFunc(){}

float LinearFunc::GetValue(Vector2 p1, Vector2 p2, float val){
    return Lerp(p1.y, p2.y, GetRate(val, p1.x, p2.x));
}

SquareFunc::SquareFunc(){}
SquareFunc::~SquareFunc(){}

float SquareFunc::GetValue(Vector2 p1, Vector2 p2, float val){
    float rate = GetRate(val, p1.x, p2.x);
    return Lerp(p1.y, p2.y, rate <= 0.5f ? 2.0f * rate * rate : rate * (-2.0f * rate + 4.0f) - 1.0f);
}

const float AnimationCurve::BOUND_TOP = 0.8f;
const float AnimationCurve::BOUND_BOTTOM = -0.8f;
const float AnimationCurve::ERROR_NUM = 10.0f;
const float AnimationCurve::DEPTH = -1.0f;

AnimationCurve::AnimationCurve(float startFrame, float endFrame) : startFrame(startFrame), endFrame(endFrame) {
    points.Add(Vector2(startFrame, 0.0f));
    points.Add(Vector2(endFrame, 0.0f));

    functions.Add(new IAnimationFunction());

    basicMenu = new Menu();
    basicMenu->AddItem(new MenuItem(L"添加点", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->AddPoint(curve->selIndex, Vector2(Lerp(curve->startFrame, curve->endFrame, (curve->cursorPos.x + 1.0f) * 0.5f), curve->cursorPos.y / curve->ratio));
        curve->selTarget = NONE;
    }, this));
    basicMenu->AddItem(new MenuItem(L"删除点", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        if (curve->selIndex > 0){
            curve->RemovePoint(curve->selIndex);
        }
        curve->selTarget = NONE;
    }, this));
    basicMenu->AddItem(new MenuItem());

    Menu* rangeMenu = new Menu();
    rangeMenu->AddItem(new MenuItem(L"刷新范围", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->FlushRange();
    }, this));
    rangeMenu->AddItem(new MenuItem(L"范围2倍", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->ratio *= 0.5f;
    }, this));
    rangeMenu->AddItem(new MenuItem(L"范围10倍", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->ratio *= 0.1f;
    }, this));
    rangeMenu->AddItem(new MenuItem(L"范围100倍", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->ratio *= 0.01f;
    }, this));
    rangeMenu->AddItem(new MenuItem(L"范围0.5倍", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->ratio *= 2.0f;
    }, this));
    rangeMenu->AddItem(new MenuItem(L"范围0.1倍", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->ratio *= 10.0f;
    }, this));
    rangeMenu->AddItem(new MenuItem(L"范围0.01倍", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->ratio *= 100.0f;
    }, this));
    basicMenu->AddItem(new MenuItem(L"范围", rangeMenu));

    Menu* funcMenu = new Menu();
    funcMenu->AddItem(new MenuItem(L"常量", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->SetFunc(curve->selIndex, new IAnimationFunction());
    }, this));
    funcMenu->AddItem(new MenuItem(L"右方常量", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->SetFunc(curve->selIndex, new RightValueFunc());
    }, this));
    funcMenu->AddItem(new MenuItem(L"线性", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->SetFunc(curve->selIndex, new LinearFunc());
    }, this));
    funcMenu->AddItem(new MenuItem(L"平方", MENUITEM_LAMBDA_TRANS(AnimationCurve)[](AnimationCurve* curve){
        curve->SetFunc(curve->selIndex, new SquareFunc());
    }, this));
    basicMenu->AddItem(new MenuItem(L"函数", funcMenu));
}

AnimationCurve::~AnimationCurve(){
    Free(functions);
    if (basicMenu) delete basicMenu;
}

bool AnimationCurve::Trigger(Vector2 pos){
    return pos.y >= BOUND_BOTTOM && pos.y <= BOUND_TOP;
}

void AnimationCurve::Hover(Vector2 pos){
    cursorPos = pos;
}

void AnimationCurve::Click(Vector2 pos){
    ViewManager* viewMgr = ViewManager::GetLocalInst();
    size_t size = points.Size();
    size_t seg;
    float err;

    cursorPos = pos;
    err = (4.0f * ERROR_NUM * ERROR_NUM) / Square(viewMgr->GetHeight());

    for (size_t i = 1; i < size - 1; i++){
        Vector2 pointPos = Vector2(GetRate(points[i].x, startFrame, endFrame) * 2.0f - 1.0f, points[i].y * ratio);
        if ((pointPos - pos).SqrMagnitude() <= err){
            selTarget = POINT;
            selIndex = i;
            initialPos = pointPos;
            DebugLog("AnimationCurve::Click Select Point %d", selIndex);
            return;
        }
    }

    seg = GetSegment(Lerp(startFrame, endFrame, (pos.x + 1.0f) * 0.5f));
    if (seg != -1){
        if (selTarget == SEGMENT && selIndex == seg){
            Main::SetMenu(basicMenu);
            selTarget = NONE;
            return;
        }
        selTarget = SEGMENT;
        selIndex = seg;
        DebugLog("AnimationCurve::Click Select Segment %d", selIndex);
        return;
    }

    selTarget = NONE;
    DebugLog("AnimationCurve::Click Select None");
}

void AnimationCurve::Drag(Vector2 dir){
    if (selTarget == POINT){
        Vector2 pos = initialPos + dir;
        points[selIndex] = Vector2(
            Clamp(Lerp(startFrame, endFrame, (pos.x + 1.0f) * 0.5f), points[selIndex - 1].x, points[selIndex + 1].x),
            pos.y / ratio
        );
    }
}

void AnimationCurve::OnWheel(int delta){
    ratio *= Pow(1.001f, delta);
}

void AnimationCurve::Render(){
    size_t size;

    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(-1.0f, -0.8f, DEPTH);
    glVertex3f(1.0f, -0.8f, DEPTH);
    glVertex3f(1.0f, 0.9f, DEPTH);
    glVertex3f(-1.0f, 0.9f, DEPTH);
    glEnd();

    glPushMatrix();
    glScalef(1.0f, ratio, 1.0f);

    // 动画曲线绘制
    glEnable(GL_LINE_SMOOTH);
    if (selTarget == SEGMENT){
        glLineWidth(4.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (float pos = points[selIndex].x; pos <= points[selIndex + 1].x + 0.5f; pos += 1.0f){
            glVertex3f(GetRate(pos, startFrame, endFrame) * 2.0f - 1.0f, GetValue(pos), DEPTH);
        }
        glEnd();
    }
    glLineWidth(3.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (float pos = startFrame; pos <= endFrame + 0.5f; pos += 1.0f){
        glVertex3f(GetRate(pos, startFrame, endFrame) * 2.0f - 1.0f, GetValue(pos), DEPTH);
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
        glVertex3f(GetRate(p.x, curve->startFrame, curve->endFrame) * 2.0f - 1.0f, p.y, DEPTH);
    }, this);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    size = points.Size() - 1;
    for (size_t i = 1; i < size; i++){
        Vector2 p = points[i];
        float pos = GetRate(p.x, startFrame, endFrame) * 2.0f - 1.0f;
        char s[40];

        __builtin_snprintf(s, 40, "(%d,%f)", (int)Floor(p.x), p.y);
        glRasterPos3f(pos, p.y, DEPTH);
        glDrawString(s);
    }

    if (selTarget == POINT){
        glPointSize(8.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertex3f(GetRate(points[selIndex].x, startFrame, endFrame) * 2.0f - 1.0f, points[selIndex].y, DEPTH);
        glEnd();
    }
    glDisable(GL_POINT_SMOOTH);
    glPointSize(1.0f);

    glPopMatrix();
}

size_t AnimationCurve::GetSegment(float pos){
    size_t size = points.Size();

    for (size_t i = 0; i < size - 1; i++){
        if (pos >= points[i].x && pos <= points[i + 1].x){
            return i;
        }
    }

    return -1;
}

float AnimationCurve::GetValue(float pos){
    size_t seg = GetSegment(pos);

    if (seg == -1){
        return 0.0f;
    }

    return functions[seg]->GetValue(points[seg], points[seg + 1], pos);
}

void AnimationCurve::OnChangeRange(float start, float end){
    points[0].x = start;
    points[points.Size() - 1].x = end;
}

void AnimationCurve::SetFunc(size_t seg, IAnimationFunction* func){
    if (functions[seg])
        delete functions[seg];
    functions[seg] = func;
}

void AnimationCurve::AddPoint(Vector2 point){
    AddPoint(GetSegment(point.x), point);
}

void AnimationCurve::AddPoint(size_t index, Vector2 point){
    functions.Insert(index + 1, new IAnimationFunction());
    points.Insert(index + 1, point);
}

void AnimationCurve::RemovePoint(size_t index){
    points.RemoveAt(index);
    if (functions[index]){
        delete functions[index];
    }
    functions.RemoveAt(index);
}

void AnimationCurve::FlushRange(){
    size_t size = points.Size();
    float x = 1.0f;

    for (size_t i = 1; i < size - 1; i++){
        if (Abs(points[i].y) > x)
            x = Abs(points[i].y);
    }
    ratio = 0.8f / x;
}