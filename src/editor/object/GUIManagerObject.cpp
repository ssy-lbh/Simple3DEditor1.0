#include <editor/object/GUIManagerObject.h>

#include <glew/gl/glew.h>

#include <main.h>
#include <util/gl/GLSimplified.h>
#include <util/gl/GLUtils.h>
#include <util/gl/GLTexture2D.h>
#include <util/os/Log.h>
#include <util/os/Font.h>
#include <editor/main/ViewManager.h>
#include <editor/object/GUIObject.h>

namespace simple3deditor {

OBJECT_INFO_DECL(simple3deditor::GUIManagerObject, L"GUI管理器");

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
    if (focus){
        focus->OnMouseMove2D(cursorPos);
        if (leftDown){
            focus->OnLeftDrag2D(cursorPos - startPos);
            return;
        }
        if (rightDown){
            focus->OnRightDrag2D(cursorPos - startPos);
            return;
        }
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
    if (focus){
        leftDown = true;
        focus->OnLeftDown2D(cursorPos);
    }
}

void GUIManagerObject::OnLeftUp2D(Point2 pos){
    UpdateCursor(pos);
    if (cur){
        AGUIObject* preObj = cur;
        FindCurrent();
        preObj->OnLeftUp2D(cursorPos);
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
    if (focus){
        rightDown = true;
        focus->OnRightDown2D(cursorPos);
    }
}

void GUIManagerObject::OnRightUp2D(Point2 pos){
    UpdateCursor(pos);
    if (cur){
        AGUIObject* preObj = cur;
        FindCurrent();
        preObj->OnRightUp2D(cursorPos);
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

void GUIManagerObject::OnDropFileA(const char* path, uint len){
    if (cur)
        cur->OnDropFileA(path, len);
}

void GUIManagerObject::OnDropFileW(const wchar_t* path, uint len){
    if (cur)
        cur->OnDropFileW(path, len);
}

AGUIObject* GUIManagerObject::GetCurrent(){
    return cur;
}

AGUIObject* GUIManagerObject::FindCurrent(){
    cur = NULL;
    children.Foreach([=](AViewObject* o){
        if(o->OnHit2D(this->cursorPos)){
            try {
                this->cur = dynamic_cast<AGUIObject*>(o);
                o->OnMouseMove2D(this->cursorPos);
            }catch(std::bad_cast e){
                DebugError("GUIManagerObject::FindCurrent Current Object Is Not GUI Object");
            }
        }
    });
    return cur;
}

}