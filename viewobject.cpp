#include <viewobject.h>

#include <main.h>
#include <res.h>

Transform::Transform(){}
Transform::~Transform(){}

Quaternion Transform::GetRotation(){
    if (rotationMode == ROT_QUATERNION)
        return rotation.Normal();

    Quaternion rotX = Quaternion::AxisAngle(Vector3::right, rotationXYZ.x);
    Quaternion rotY = Quaternion::AxisAngle(Vector3::forward, rotationXYZ.y);
    Quaternion rotZ = Quaternion::AxisAngle(Vector3::up, rotationXYZ.z);

    switch (rotationMode){
    case ROT_EULER_XYZ: return rotZ * rotY * rotX;
    case ROT_EULER_XZY: return rotY * rotZ * rotX;
    case ROT_EULER_YXZ: return rotZ * rotX * rotY;
    case ROT_EULER_YZX: return rotX * rotZ * rotY;
    case ROT_EULER_ZXY: return rotY * rotX * rotZ;
    case ROT_EULER_ZYX: return rotX * rotY * rotZ;
    }

    DebugError("Transform::GetRotation Unknown Rotation Mode %d", rotationMode);

    return Quaternion::one;
}

Matrix4x4 Transform::GetMatrix(){
    Matrix4x4 mat = Matrix4x4::identity;

    mat._11 = scale.x;
    mat._22 = scale.y;
    mat._33 = scale.z;

    if (rotationMode == ROT_QUATERNION){
        mat *= rotation.Normal();
    }else{
        Quaternion rotX = Quaternion::AxisAngle(Vector3::right, rotationXYZ.x);
        Quaternion rotY = Quaternion::AxisAngle(Vector3::forward, rotationXYZ.y);
        Quaternion rotZ = Quaternion::AxisAngle(Vector3::up, rotationXYZ.z);

        switch (rotationMode){
        case ROT_EULER_XYZ:
            mat *= (rotZ * rotY * rotX);
            break;
        case ROT_EULER_XZY:
            mat *= (rotY * rotZ * rotX);
            break;
        case ROT_EULER_YXZ:
            mat *= (rotZ * rotX * rotY);
            break;
        case ROT_EULER_YZX:
            mat *= (rotX * rotZ * rotY);
            break;
        case ROT_EULER_ZXY:
            mat *= (rotY * rotX * rotZ);
            break;
        case ROT_EULER_ZYX:
            mat *= (rotX * rotY * rotZ);
            break;
        }
    }

    mat._14 = position.x;
    mat._24 = position.y;
    mat._34 = position.z;

    // 调试输出矩阵
    // DebugLog("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f",
    //          mat._11, mat._12, mat._13, mat._14,
    //          mat._21, mat._22, mat._23, mat._24,
    //          mat._31, mat._32, mat._33, mat._34,
    //          mat._41, mat._42, mat._43, mat._44);

    return mat;
}

Matrix4x4 Transform::GetInvMatrix(){
    Matrix4x4 mat = Matrix4x4::identity;

    mat._14 = -position.x;
    mat._24 = -position.y;
    mat._34 = -position.z;

    if (rotationMode == ROT_QUATERNION){
        mat *= -rotation.Normal();
    }else{
        Quaternion rotX = Quaternion::AxisAngle(Vector3::right, rotationXYZ.x);
        Quaternion rotY = Quaternion::AxisAngle(Vector3::forward, rotationXYZ.y);
        Quaternion rotZ = Quaternion::AxisAngle(Vector3::up, rotationXYZ.z);

        switch (rotationMode){
        case ROT_EULER_XYZ:
            mat *= -(rotZ * rotY * rotX);
            break;
        case ROT_EULER_XZY:
            mat *= -(rotY * rotZ * rotX);
            break;
        case ROT_EULER_YXZ:
            mat *= -(rotZ * rotX * rotY);
            break;
        case ROT_EULER_YZX:
            mat *= -(rotX * rotZ * rotY);
            break;
        case ROT_EULER_ZXY:
            mat *= -(rotY * rotX * rotZ);
            break;
        case ROT_EULER_ZYX:
            mat *= -(rotX * rotY * rotZ);
            break;
        }
    }

    float tmp;

    tmp = 1.0f / scale.x; mat._11 *= tmp; mat._12 *= tmp; mat._13 *= tmp;
    tmp = 1.0f / scale.y; mat._21 *= tmp; mat._22 *= tmp; mat._23 *= tmp;
    tmp = 1.0f / scale.z; mat._31 *= tmp; mat._32 *= tmp; mat._33 *= tmp;

    return mat;
}

void Transform::PushMatrix(){
    GLUtils::PushMatrix(GetMatrix());
}

void Transform::PushInvMatrix(){
    GLUtils::PushMatrix(GetInvMatrix());
}

void Transform::PopMatrix(){
    GLUtils::PopMatrix();
}

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

Matrix4x4 AViewObject::GetObjectToWorldMatrix(){
    if (parent)
        return transform.GetMatrix() * parent->GetObjectToWorldMatrix();
    return transform.GetMatrix();
}

Matrix4x4 AViewObject::GetWorldToObjectMatrix(){
    if (parent)
        return transform.GetInvMatrix() * parent->GetWorldToObjectMatrix();
    return transform.GetInvMatrix();
}

void AViewObject::OnSelect(Vector3 ori, Vector3 dir){}
void AViewObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){}
void AViewObject::OnSelectUV(Vector2 uv, float err){}
void AViewObject::OnSelectUV(Vector2 uv1, Vector2 uv2){}

Mesh* AViewObject::GetMesh(){ return NULL; }

void AViewObject::OnRender(const RenderOptions* options){
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        children[i]->OnRender(options);
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
    mesh = new Mesh(this);
}

MeshObject::~MeshObject(){
    // 子类析构器在返回前会自动调用父类析构器
    // 因此不要再这里调用AViewObject::~AViewObject，不然会导致多次delete使程序崩溃
    Free(children);
    if (mesh) delete mesh;
}

void MeshObject::OnSelect(Vector3 ori, Vector3 dir){
    Matrix4x4 curMat = transform.GetInvMatrix();

    ori = curMat * Vector4(ori, 1.0f);
    dir = curMat * Vector4(dir, 0.0f);
    AViewObject::OnSelect(ori, dir);

    switch (Main::data->selType){
    case GlobalData::SELECT_VERTICES:{
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
    case GlobalData::SELECT_EDGES:{
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
    Matrix4x4 curMat = transform.GetInvMatrix();

    camPos = curMat * Vector4(camPos, 1.0f);
    camDir = transform.GetRotation() * camDir;
    AViewObject::OnSelect(camPos, camDir, zBound, p1, p2);

    switch (Main::data->selType){
    case GlobalData::SELECT_VERTICES:
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

void MeshObject::OnRender(const RenderOptions* options){
    transform.PushMatrix();
    AViewObject::OnRender(options);

    mesh->Render(options->light);

    transform.PopMatrix();
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
    Free(children);
}

void BezierCurveObject::OnSelect(Vector3 ori, Vector3 dir){
    Matrix4x4 curMat = transform.GetInvMatrix();

    ori = curMat * Vector4(ori, 1.0f);
    dir = curMat * Vector4(dir, 0.0f);
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
    Matrix4x4 curMat = transform.GetInvMatrix();

    camPos = curMat * Vector4(camPos, 1.0f);
    camDir = transform.GetRotation() * camDir;
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

void BezierCurveObject::OnRender(const RenderOptions* options){
    AViewObject::OnRender(options);
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
    Free(children);
    GLLights::DestroyLight(light);
}

void PointLightObject::OnSelect(Vector3 ori, Vector3 dir){
    Matrix4x4 curMat = transform.GetInvMatrix();

    ori = curMat * Vector4(ori, 1.0f);
    dir = curMat * Vector4(dir, 0.0f);
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
    Matrix4x4 curMat = transform.GetInvMatrix();

    camPos = curMat * Vector4(camPos, 1.0f);
    camDir = transform.GetRotation() * camDir;
    AViewObject::OnSelect(camPos, camDir, zBound, p1, p2);

    if (v.Hit(camPos, camDir, zBound, p1, p2)){
        Main::data->selPoints.Add(&v);
        DebugLog("Select Point %f %f %f", v.pos.x, v.pos.y, v.pos.z);
    }else{
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void PointLightObject::OnRender(const RenderOptions* options){
    AViewObject::OnRender(options);
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
    Free(children);
}

void AudioListenerObject::OnSelect(Vector3 ori, Vector3 dir){
    Matrix4x4 curMat = transform.GetInvMatrix();

    ori = curMat * Vector4(ori, 1.0f);
    dir = curMat * Vector4(dir, 0.0f);
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
    Matrix4x4 curMat = transform.GetInvMatrix();

    camPos = curMat * Vector4(camPos, 1.0f);
    camDir = transform.GetRotation() * camDir;
    AViewObject::OnSelect(camPos, camDir, zBound, p1, p2);

    if (v.Hit(camPos, camDir, zBound, p1, p2)){
        Main::data->selPoints.Add(&v);
        DebugLog("Select Point %f %f %f", v.pos.x, v.pos.y, v.pos.z);
    }else{
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void AudioListenerObject::OnRender(const RenderOptions* options){
    AViewObject::OnRender(options);
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