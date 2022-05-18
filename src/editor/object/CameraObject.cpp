#include <editor/object/CameraObject.h>

#include <main.h>
#include <util/gl/GLSimplified.h>

namespace simple3deditor {

static const float SCALE = 0.3f;

CameraObject::CameraObject() : AViewObject(L"Camera", ViewObjectType::OBJECT_CAMERA) {
    pos.object = this;
    lookAt.object = this;
}

CameraObject::~CameraObject(){
    LocalData::GetLocalInst()->camera = NULL;
}

void CameraObject::OnSelect(Point3 ori, Vector3 dir){
    ori = transform.chainInvMat * ori;
    dir = transform.chainInvMat * dir;
    
    if (pos.Hit(ori, dir)){
        if (Main::data->selPoints.HasValue(&pos))
            return;
        Main::data->selPoints.Add(&pos);
        DebugLog("Select Point %f %f %f", pos.pos.x, pos.pos.y, pos.pos.z);
    }else if (lookAt.Hit(ori, dir)){
        if (Main::data->selPoints.HasValue(&lookAt))
            return;
        Main::data->selPoints.Add(&lookAt);
        DebugLog("Select Point %f %f %f", lookAt.pos.x, lookAt.pos.y, lookAt.pos.z);
    }else{
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void CameraObject::OnSelect(const SelectInfo* info){
    bool hit = false;

    if (info->Inside(transform.chainMat * pos.pos)){
        hit = true;
        if (Main::data->selPoints.HasValue(&pos))
            return;
        Main::data->selPoints.Add(&pos);
        DebugLog("Select Point %f %f %f", pos.pos.x, pos.pos.y, pos.pos.z);
    }
    
    if (info->Inside(transform.chainMat * lookAt.pos)){
        hit = true;
        if (Main::data->selPoints.HasValue(&lookAt))
            return;
        Main::data->selPoints.Add(&lookAt);
        DebugLog("Select Point %f %f %f", lookAt.pos.x, lookAt.pos.y, lookAt.pos.z);
    }
    
    if (!hit){
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void CameraObject::OnRender(){
    AViewObject::OnRender();

    Quaternion dir;
    Vector3 forward;
    Vector3 right;
    Vector3 up;

    Vector3 v = pos.pos;
    Vector3 v1, v2, v3, v4;

    if ((lookAt.pos - pos.pos).SqrMagnitude() == 0.0f)
        return;
    forward = (lookAt.pos - pos.pos).Normal();
    dir = Quaternion::FromTo(Vector3::forward, forward);
    right = dir * Vector3::right;
    up = dir * Vector3::up;

    v1 = Vector3::Compose(Vector3(SCALE, SCALE, SCALE), right, forward, up);
    v2 = Vector3::Compose(Vector3(SCALE, SCALE, -SCALE), right, forward, up);
    v3 = Vector3::Compose(Vector3(-SCALE, SCALE, SCALE), right, forward, up);
    v4 = Vector3::Compose(Vector3(-SCALE, SCALE, -SCALE), right, forward, up);

    glEnable(GL_LINE_SMOOTH);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertexv3(v1);
    glVertexv3(v2);
    glVertexv3(v4);
    glVertexv3(v3);
    glEnd();
    glBegin(GL_LINES);
    glVertexv3(v); glVertexv3(v1);
    glVertexv3(v); glVertexv3(v2);
    glVertexv3(v); glVertexv3(v3);
    glVertexv3(v); glVertexv3(v4);
    glEnd();
    glDisable(GL_LINE_SMOOTH);

    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(4.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    glVertexv3(v);
    glEnd();
    glDisable(GL_POINT_SMOOTH);
}

}