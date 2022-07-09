#include <editor/object/CubicBezierObject.h>

#include <main.h>
#include <util/math3d/Mesh.h>
#include <util/gl/GLSimplified.h>
#include <util/gl/GLUtils.h>

namespace simple3deditor {

CubicBezierObject::CubicBezierObject() : AViewObject(L"CubicBezier", ViewObjectType::OBJECT_CUBIC_BEZIER_CURVE) {
    v[0].pos = Vector3(-5.0f, 0.0f, 0.0f);
    v[1].pos = Vector3(0.0f, 0.0f, 0.0f);
    v[2].pos = Vector3(0.0f, 0.0f, 5.0f);
    v[3].pos = Vector3(5.0f, 0.0f, 5.0f);
    for (int i = 0; i < 4; i++)
        v[i].object = this;
}

CubicBezierObject::~CubicBezierObject(){}

void CubicBezierObject::OnSelect(Point3 ori, Vector3 dir){
    ori = transform.chainInvMat * ori;
    dir = transform.chainInvMat * dir;

    bool hit = false;
    for (int i = 0; i < 4; i++){
        if (v[i].Hit(ori, dir)){
            if (Main::data->selPoints.HasValue(&v[i]))
                return;
            Main::data->selPoints.Add(&v[i]);
            DebugLog("Select Point %f %f %f", v[i].pos.x, v[i].pos.y, v[i].pos.z);
            hit = true;
        }
    }
    if (!hit){
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void CubicBezierObject::OnSelect(const SelectInfo* info){
    bool hit = false;
    for (int i = 0; i < 4; i++){
        if (info->Inside(transform.chainMat * Point3(v[i].pos))){
            if (Main::data->selPoints.HasValue(&v[i]))
                return;
            Main::data->selPoints.Add(&v[i]);
            DebugLog("Select Point %f %f %f", v[i].pos.x, v[i].pos.y, v[i].pos.z);
            hit = true;
        }
    }
    if (!hit){
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void CubicBezierObject::OnSelectUV(Vector2 uv, float err){
    bool hit = false;
    for (int i = 0; i < 4; i++){
        if (v[i].HitUV(uv, err)){
            if (Main::data->selPoints.HasValue(&v[i]))
                return;
            Main::data->selPoints.Add(&v[i]);
            DebugLog("Select Point %f %f", v[i].uv.x, v[i].uv.y);
            hit = true;
        }
    }
    if (!hit){
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void CubicBezierObject::OnSelectUV(Vector2 uv1, Vector2 uv2){
    for (int i = 0; i < 4; i++){
        if (v[i].HitUV(uv1, uv2))
            Main::data->selPoints.Add(&v[i]);
    }
}

void CubicBezierObject::OnRender(){
    AViewObject::OnRender();

    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(4.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++){
        glVertex3f(v[i].pos.x, v[i].pos.y, v[i].pos.z);
    }
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    GLUtils::DrawBezier(v[0].pos, v[1].pos, v[2].pos, v[3].pos, 0.01f);
    glDisable(GL_LINE_SMOOTH);
}

void CubicBezierObject::OnRenderUVMap(){
    AViewObject::OnRenderUVMap();

    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(4.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++)
        glVertex2f(v[i].uv.x, v[i].uv.y);
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    GLUtils::DrawBezier(v[0].uv, v[1].uv, v[2].uv, v[3].uv, 0.01f);
    glDisable(GL_LINE_SMOOTH);
}

}