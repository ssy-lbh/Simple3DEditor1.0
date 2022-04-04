#include <editor/object/GUIManagerObject.h>

#include <lib/glew/gl/glew.h>

#include <exception>

#include <main.h>
#include <utils/gl/GLSimplified.h>
#include <utils/gl/GLUtils.h>
#include <utils/gl/GLTexture2D.h>
#include <utils/os/Log.h>
#include <utils/os/Font.h>
#include <editor/gui/ViewManager.h>
#include <editor/object/GUIObject.h>

GUIManagerObject::GUIManagerObject() : AGUIObject(L"GUIManager", ViewObjectType::OBJECT_GUI_MANAGER) {}

GUIManagerObject::GUIManagerObject(const wchar_t* name, ViewObjectType type) : AGUIObject(name, type) {}

GUIManagerObject::~GUIManagerObject(){}

bool GUIManagerObject::UpdateCursor3D(Point3 ori, Vector3 dir){
    Point3 camPos = transform.chainInvMat * ori;
    Vector3 cursorDir = transform.chainInvMat * dir;
    // 与XY上的正方形相交
    if (cursorDir.z == 0.0f)
        return false;
    // 暂且不知道怎么不重复赋值
    cursorPos = (camPos - cursorDir * (camPos.z / cursorDir.z));
    return true;
}

void GUIManagerObject::UpdateCursor(Vector2 pos){
    cursorPos = pos;
}

void GUIManagerObject::AddChild(AViewObject* o){
    try {
        AGUIObject* gui = dynamic_cast<AGUIObject*>(o);
        AViewObject::AddChild(o);
        gui->SetGUIManager(this);
    }catch(std::bad_cast e){
        DebugError("Add A None GUI Object To GUIManagerObject");
    }
}

bool GUIManagerObject::DeleteChild(AViewObject* o){
    if (!AViewObject::DeleteChild(o)){
        DebugLog("GUIManagerObject::DeleteChild Failed");
        return false;
    }
    if (cur == o){
        cur = NULL;
    }
    return true;
}

void GUIManagerObject::OnRender(){
    AViewObject::OnRender();
    
    // 测试代码
    // glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
    // GLUtils::DrawRect(Rect(-1.0f, 1.0f, -1.0f, 1.0f));
}

void GUIManagerObject::OnMouseMove(Point3 ori, Vector3 dir){
    if (UpdateCursor3D(ori, dir))
        OnMouseMove2D(cursorPos);
}

void GUIManagerObject::OnLeftDown(Point3 ori, Vector3 dir){
    if (UpdateCursor3D(ori, dir))
        OnLeftDown2D(cursorPos);
}

void GUIManagerObject::OnLeftUp(Point3 ori, Vector3 dir){
    if (UpdateCursor3D(ori, dir))
        OnLeftUp2D(cursorPos);
}

void GUIManagerObject::OnRightDown(Point3 ori, Vector3 dir){
    if (UpdateCursor3D(ori, dir))
        OnRightDown2D(cursorPos);
}

void GUIManagerObject::OnRightUp(Point3 ori, Vector3 dir){
    if (UpdateCursor3D(ori, dir))
        OnRightUp2D(cursorPos);
}

void GUIManagerObject::OnMouseMove2D(Point2 pos){
    UpdateCursor(pos);
    if (leftDown && cur){
        cur->OnLeftDrag2D(cursorPos - startPos);
        return;
    }
    if (rightDown && cur){
        cur->OnRightDrag2D(cursorPos - startPos);
        return;
    }
    FindCurrent();
}

void GUIManagerObject::OnLeftDown2D(Point2 pos){
    UpdateCursor(pos);
    FindCurrent();
    startPos = cursorPos;
    if (focus != cur){
        if (focus)
            focus->OnKillFocus();
        focus = cur;
        if (focus)
            focus->OnFocus();
    }
    if (cur){
        leftDown = true;
        cur->OnLeftDown2D(cursorPos);
    }
}

void GUIManagerObject::OnLeftUp2D(Point2 pos){
    UpdateCursor(pos);
    if (cur){
        cur->OnLeftUp2D(cursorPos);
        FindCurrent();
    }
    if (leftDown)
        leftDown = false;
}

void GUIManagerObject::OnRightDown2D(Point2 pos){
    UpdateCursor(pos);
    FindCurrent();
    startPos = cursorPos;
    if (focus != cur){
        if (focus)
            focus->OnKillFocus();
        focus = cur;
        if (focus)
            focus->OnFocus();
    }
    if (cur){
        rightDown = true;
        cur->OnRightDown2D(cursorPos);
    }
}

void GUIManagerObject::OnRightUp2D(Point2 pos){
    UpdateCursor(pos);
    if (cur){
        cur->OnRightUp2D(cursorPos);
        FindCurrent();
    }
    if (rightDown)
        rightDown = false;
}

void GUIManagerObject::OnMouseWheel(int delta){
    if (cur)
        cur->OnMouseWheel(delta);
}

void GUIManagerObject::OnMenuAccel(int id, bool accel){
    if (cur)
        cur->OnMenuAccel(id, accel);
}

void GUIManagerObject::OnKillFocus(){
    if (focus){
        focus->OnKillFocus();
        focus = NULL;
    }
    cur = NULL;
}

void GUIManagerObject::OnChar(char c){
    if (cur)
        cur->OnChar(c);
}

void GUIManagerObject::OnUnichar(wchar_t c){
    if (cur)
        cur->OnUnichar(c);
}

void GUIManagerObject::OnDropFileA(const char* path){
    if (cur)
        cur->OnDropFileA(path);
}

void GUIManagerObject::OnDropFileW(const wchar_t* path){
    if (cur)
        cur->OnDropFileW(path);
}

AGUIObject* GUIManagerObject::GetCurrent(){
    return cur;
}

AGUIObject* GUIManagerObject::FindCurrent(){
    cur = NULL;
    children.Foreach<GUIManagerObject*>([](AViewObject* o, GUIManagerObject* mgr){
        if(o->OnHit2D(mgr->cursorPos)){
            try {
                mgr->cur = dynamic_cast<AGUIObject*>(o);
                o->OnMouseMove2D(mgr->cursorPos);
            }catch(std::bad_cast e){
                DebugError("GUIManagerObject::FindCurrent Current Object Is Not GUI Object");
            }
        }
    }, this);
    return cur;
}