#include "viewobject.h"

#include "main.h"
#include "res.h"

AViewObject::AViewObject() : name(L"Object"){}

AViewObject::AViewObject(const wchar_t* name) : name(name){}

AViewObject::AViewObject(WString name) : name(name){}

AViewObject::~AViewObject(){
    Free(children);
}

void AViewObject::AddChild(AViewObject* o){
    if (o->parent)
        o->parent->DeleteChild(o);
    o->parent = this;
    children.Add(o);
}

bool AViewObject::DeleteChild(AViewObject* o){
    if (o->parent != this)
        return false;
    o->parent = NULL;
    return children.Remove(o);
}

void AViewObject::EnumChildren(void(*func)(AViewObject*)){
    children.Foreach(func);
}

void AViewObject::EnumChildren(void(*func)(AViewObject*, void*), void* user){
    children.Foreach<void*>(func, user);
}

AViewObject* AViewObject::GetParent(){
    return parent;
}

void AViewObject::SetParent(AViewObject* o){
    if (parent)
        parent->DeleteChild(this);
    o->AddChild(this);
}

List<AViewObject*>& AViewObject::GetChildren(){
    return children;
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

void AViewObject::OnTimer(int id){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnTimer(id);
}

void AViewObject::OnChar(char c){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnChar(c);
}

void AViewObject::OnUnichar(wchar_t c){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnUnichar(c);
}

void AViewObject::OnMenuAccel(int id, bool accel){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnMenuAccel(id, accel);
}

void AViewObject::OnAnimationFrame(int frame){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnAnimationFrame(frame);
}

MeshObject::MeshObject() : AViewObject(L"Mesh"){
    mesh = new Mesh();
}

MeshObject::~MeshObject(){
    AViewObject::~AViewObject();
    if (mesh) delete mesh;
}

void MeshObject::OnSelect(Vector3 ori, Vector3 dir){
    AViewObject::OnSelect(ori, dir);
    switch (Main::data->selType){
    case MainData::SELECT_VERTICES:{
        Vertex* v = mesh->Find(ori, dir);
        if (!v){
            Main::data->selPoints.Clear();
            DebugLog("No Point Selected");
        }else{
            Main::data->selPoints.Add(v);
            DebugLog("Select Point %f %f %f", v->pos.x, v->pos.y, v->pos.z);
        }
    }
        break;
    case MainData::SELECT_EDGES:{
        Edge* e = mesh->FindEdge(ori, dir);
        if (!e){
            Main::data->selEdges.Clear();
            DebugLog("No Edge Selected");
        }else{
            Main::data->selEdges.Add(e);
            DebugLog("Select Edge (%f,%f,%f) (%f,%f,%f)", e->v1->pos.x, e->v1->pos.y, e->v1->pos.z, e->v2->pos.x, e->v2->pos.y, e->v2->pos.z);
        }
    }
        break;
    }
}

void MeshObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){
    AViewObject::OnSelect(camPos, camDir, zBound, p1, p2);
    switch (Main::data->selType){
    case MainData::SELECT_VERTICES:
        mesh->FindScreenRect(
            camPos, camDir,
            zBound.x, zBound.y,
            p1.x, p2.x,
            p1.y, p2.y,
            Main::data->selPoints
        );
        break;
    }
}

void MeshObject::OnSelectUV(Vector2 uv, float err){
    AViewObject::OnSelectUV(uv, err);
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
    AViewObject::OnSelectUV(uv1, uv2);
    mesh->FindUVRect(uv1, uv2, Main::data->selPoints);
}

Mesh* MeshObject::GetMesh(){
    return mesh;
}

void MeshObject::OnRender(){
    AViewObject::OnRender();
    
    // 测试代码
    // glPushMatrix();
    // glTranslatef(Main::data->animFrame * 0.01f, 0.0f, 0.0f);
    // mesh->Render();
    // glPopMatrix();

    mesh->Render();
}

void MeshObject::OnRenderUVMap(){
    AViewObject::OnRenderUVMap();
    mesh->RenderUVMap();
}

BezierCurveObject::BezierCurveObject() : AViewObject(L"BezierCurve"){
    v[0].pos = Vector3(-5.0f, 0.0f, 0.0f);
    v[1].pos = Vector3(0.0f, 0.0f, 0.0f);
    v[2].pos = Vector3(0.0f, 0.0f, 5.0f);
    v[3].pos = Vector3(5.0f, 0.0f, 5.0f);
}

BezierCurveObject::~BezierCurveObject(){
    AViewObject::~AViewObject();
}

void BezierCurveObject::OnSelect(Vector3 ori, Vector3 dir){
    AViewObject::OnSelect(ori, dir);
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
    AViewObject::OnSelect(camPos, camDir, zBound, p1, p2);
    bool hit = false;
    for (int i = 0; i < 4; i++){
        if (v[i].Hit(camPos, camDir, zBound, p1, p2)){
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

void BezierCurveObject::OnSelectUV(Vector2 uv, float err){
    AViewObject::OnSelectUV(uv, err);
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
    AViewObject::OnSelectUV(uv1, uv2);
    for (int i = 0; i < 4; i++){
        if (v[i].HitUV(uv1, uv2))
            Main::data->selPoints.Add(&v[i]);
    }
}

void BezierCurveObject::OnRender(){
    AViewObject::OnRender();
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

PointLightObject::PointLightObject() : AViewObject(L"PointLight"){
    light = GLLights::CreateLight();
    UpdateLight();
}

PointLightObject::~PointLightObject(){
    AViewObject::~AViewObject();
    GLLights::DestroyLight(light);
}

void PointLightObject::OnSelect(Vector3 ori, Vector3 dir){
    AViewObject::OnSelect(ori, dir);
    if (v.Hit(ori, dir)){
        Main::data->selPoints.Add(&v);
        DebugLog("Select Point %f %f %f", v.pos.x, v.pos.y, v.pos.z);
    }else{
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void PointLightObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){
    AViewObject::OnSelect(camPos, camDir, zBound, p1, p2);
    if (v.Hit(camPos, camDir, zBound, p1, p2)){
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
    GLfloat position[] = {v.pos.x, v.pos.y, v.pos.z, 1.0};// 最后一个参数为1.0表示该光源是point light

    GLfloat ambient[] = {0.0, 0.0, 0.0, 0.0};// 暂不使用环境光
    GLfloat diffuse[] = {v.color.x, v.color.y, v.color.z, 1.0};
    GLfloat specular[] = {v.color.x, v.color.y, v.color.z, 1.0};

    glLightfv(light, GL_AMBIENT, ambient);
    glLightfv(light, GL_DIFFUSE, diffuse);
    glLightfv(light, GL_SPECULAR, specular);
    glLightfv(light, GL_POSITION, position);
}

AudioListenerObject::AudioListenerObject() : AViewObject(L"AudioListener"){}

AudioListenerObject::~AudioListenerObject(){
    AViewObject::~AViewObject();
}

void AudioListenerObject::OnSelect(Vector3 ori, Vector3 dir){
    AViewObject::OnSelect(ori, dir);
    if (v.Hit(ori, dir)){
        Main::data->selPoints.Add(&v);
        DebugLog("Select Point %f %f %f", v.pos.x, v.pos.y, v.pos.z);
    }else{
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void AudioListenerObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){
    AViewObject::OnSelect(camPos, camDir, zBound, p1, p2);
    if (v.Hit(camPos, camDir, zBound, p1, p2)){
        Main::data->selPoints.Add(&v);
        DebugLog("Select Point %f %f %f", v.pos.x, v.pos.y, v.pos.z);
    }else{
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void AudioListenerObject::OnRender(){
    AViewObject::OnRender();
    Main::data->audioPos = v.pos;
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(v.pos.x, v.pos.y, v.pos.z);
    glEnd();
    glDisable(GL_POINT_SMOOTH);
}

void AudioListenerObject::OnTimer(int id){
    AViewObject::OnTimer(id);
}