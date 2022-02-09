#include "viewobject.h"

#include "main.h"
#include "res.h"

AViewObject::AViewObject(){}

AViewObject::~AViewObject(){
    Free(children);
}

void AViewObject::AddChild(AViewObject* o){
    o->parent = this;
    children.Add(o);
}

bool AViewObject::DeleteChild(AViewObject* o){
    o->parent = NULL;
    return children.Remove(o);
}

void AViewObject::EnumChildren(void(*func)(AViewObject*)){
    children.Foreach(func);
}

void AViewObject::EnumChildren(void(*func)(AViewObject*, void*), void* user){
    children.Foreach<void*>(func, user);
}

void AViewObject::OnSelect(Vector3 ori, Vector3 dir){}
void AViewObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){}
void AViewObject::OnSelectUV(Vector2 uv, float err){}
void AViewObject::OnSelectUV(Vector2 uv1, Vector2 uv2){}

Mesh* AViewObject::GetMesh(){ return NULL; }

void AViewObject::OnRender(){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnRender();
}

void AViewObject::OnRenderUVMap(){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnRenderUVMap();
}

void AViewObject::OnTimer(int id){}
void AViewObject::OnChar(char c){}
void AViewObject::OnUnichar(wchar_t c){}
void AViewObject::OnMenuAccel(int id, bool accel){}

void AViewObject::OnAnimationFrame(int frame){}

MeshObject::MeshObject(){
    mesh = new Mesh();
}

MeshObject::~MeshObject(){
    AViewObject::~AViewObject();
    if (mesh) delete mesh;
}

void MeshObject::OnSelect(Vector3 ori, Vector3 dir){
    Vertex* v = mesh->Find(ori, dir);
    if (!v){
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }else{
        Main::data->selPoints.Add(v);
        DebugLog("Select Point %f %f %f", v->pos.x, v->pos.y, v->pos.z);
    }
}

void MeshObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){
    mesh->FindScreenRect(
        camPos, camDir,
        zBound.x, zBound.y,
        p1.x, p2.x,
        p1.y, p2.y,
        Main::data->selPoints
    );
}

void MeshObject::OnSelectUV(Vector2 uv, float err){
    Vertex* v = mesh->FindUV(uv, err);
    if (!v){
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }else{
        Main::data->selPoints.Add(v);
        DebugLog("Select Point %f %f", v->uv.x, v->uv.y);
    }
}

void MeshObject::OnSelectUV(Vector2 uv1, Vector2 uv2){
    mesh->FindUVRect(uv1, uv2, Main::data->selPoints);
}

Mesh* MeshObject::GetMesh(){
    return mesh;
}

void MeshObject::OnRender(){
    AViewObject::OnRender();
    mesh->Render();
}

void MeshObject::OnRenderUVMap(){
    AViewObject::OnRenderUVMap();
    mesh->RenderUVMap();
}

BezierCurveObject::BezierCurveObject(){
    v[0].pos = Vector3(-5.0f, 0.0f, 0.0f);
    v[1].pos = Vector3(0.0f, 0.0f, 0.0f);
    v[2].pos = Vector3(0.0f, 0.0f, 5.0f);
    v[3].pos = Vector3(5.0f, 0.0f, 5.0f);
}

BezierCurveObject::~BezierCurveObject(){
    AViewObject::~AViewObject();
}

void BezierCurveObject::OnSelect(Vector3 ori, Vector3 dir){
    bool hit = false;
    for (int i = 0; i < 4; i++){
        if (v[i].Hit(ori, dir)){
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
void BezierCurveObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){
    for (int i = 0; i < 4; i++){
        if (v[i].Hit(camPos, camDir, zBound, p1, p2))
            Main::data->selPoints.Add(&v[i]);
    }
}

void BezierCurveObject::OnSelectUV(Vector2 uv, float err){
    bool hit = false;
    for (int i = 0; i < 4; i++){
        if (v[i].HitUV(uv, err)){
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

void BezierCurveObject::OnSelectUV(Vector2 uv1, Vector2 uv2){
    for (int i = 0; i < 4; i++){
        if (v[i].HitUV(uv1, uv2))
            Main::data->selPoints.Add(&v[i]);
    }
}

void BezierCurveObject::OnRender(){
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(4.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++)
        glVertex3f(v[i].pos.x, v[i].pos.y, v[i].pos.z);
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    GLUtils::DrawBezier(v[0].pos, v[1].pos, v[2].pos, v[3].pos, 0.01f);
    glDisable(GL_LINE_SMOOTH);
}

void BezierCurveObject::OnRenderUVMap(){
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