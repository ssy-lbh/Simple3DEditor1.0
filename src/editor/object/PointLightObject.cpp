#include <editor/object/PointLightObject.h>

#include <main.h>
#include <util/gl/GLSimplified.h>
#include <util/gl/GLLights.h>

namespace simple3deditor {

OBJECT_INFO_DECL(simple3deditor::PointLightObject, L"点光源");

PointLightObject::PointLightObject() : AViewObject(L"PointLight", ViewObjectType::OBJECT_POINT_LIGHT) {
    light = GLLights::Create();
    UpdateLight();
    v.object = this;
}

PointLightObject::~PointLightObject(){
    GLLights::Destroy(light);
}

void PointLightObject::OnSelect(Point3 ori, Vector3 dir){
    ori = transform.chainInvMat * ori;
    dir = transform.chainInvMat * dir;

    if (v.Hit(ori, dir)){
        if (Main::data->selPoints.HasValue(&v))
            return;
        Main::data->selPoints.Add(&v);
        DebugLog("Select Point %f %f %f", v.pos.x, v.pos.y, v.pos.z);
    }else{
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void PointLightObject::OnSelect(const SelectInfo* info){
    if (info->Inside(transform.chainMat * Point3(v.pos))){
        if (Main::data->selPoints.HasValue(&v))
            return;
        Main::data->selPoints.Add(&v);
        DebugLog("Select Point %f %f %f", v.pos.x, v.pos.y, v.pos.z);
    }else{
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void PointLightObject::OnRender(){
    AViewObject::OnRender();

    UpdateLight();
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glColor3f(v.color.x, v.color.y, v.color.z);
    glVertex3f(v.pos.x, v.pos.y, v.pos.z);
    glEnd();
    glDisable(GL_POINT_SMOOTH);
}

void PointLightObject::OnTimer(int id){
    AViewObject::OnTimer(id);
}

void PointLightObject::UpdateLight(){
    Vector3 pos = transform.GetWorldTranslation();

    GLfloat position[] = {pos.x, pos.y, pos.z, 1.0};// 最后一个参数为1.0表示该光源是point light

    GLfloat ambient[] = {0.0, 0.0, 0.0, 0.0};// 暂不使用环境光
    GLfloat diffuse[] = {v.color.x, v.color.y, v.color.z, 1.0};
    GLfloat specular[] = {v.color.x, v.color.y, v.color.z, 1.0};

    glLightfv(light, GL_AMBIENT, ambient);
    glLightfv(light, GL_DIFFUSE, diffuse);
    glLightfv(light, GL_SPECULAR, specular);
    glLightfv(light, GL_POSITION, position);
}

}