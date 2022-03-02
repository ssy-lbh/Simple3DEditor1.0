#include <utils/math3d/ViewObject.h>

#include <main.h>
#include <res.h>
#include <utils/math3d/Math.h>
#include <utils/math3d/Mesh.h>
#include <utils/gl/GLSimplified.h>
#include <utils/gl/GLLights.h>
#include <utils/gl/GLUtils.h>

Transform::Transform() : position(Vector3::zero), rotation(Quaternion::one), rotationXYZ(Vector3::zero), scale(Vector3::one) {}
Transform::~Transform(){}

Quaternion Transform::GetRotation(){
    if (rotationMode == ROT_QUATERNION)
        return rotation.Get().Normal();

    Quaternion rotX = Quaternion::AxisAngle(Vector3::right, rotationXYZ.x.Get());
    Quaternion rotY = Quaternion::AxisAngle(Vector3::forward, rotationXYZ.y.Get());
    Quaternion rotZ = Quaternion::AxisAngle(Vector3::up, rotationXYZ.z.Get());

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

    mat._11 = scale.x.Get();
    mat._22 = scale.y.Get();
    mat._33 = scale.z.Get();

    if (rotationMode == ROT_QUATERNION){
        mat *= rotation.Get().Normal();
    }else{
        Quaternion rotX = Quaternion::AxisAngle(Vector3::right, rotationXYZ.x.Get());
        Quaternion rotY = Quaternion::AxisAngle(Vector3::forward, rotationXYZ.y.Get());
        Quaternion rotZ = Quaternion::AxisAngle(Vector3::up, rotationXYZ.z.Get());

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

    mat._14 = position.x.Get();
    mat._24 = position.y.Get();
    mat._34 = position.z.Get();

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

    mat._14 = -position.x.Get();
    mat._24 = -position.y.Get();
    mat._34 = -position.z.Get();

    if (rotationMode == ROT_QUATERNION){
        mat *= -rotation.Get().Normal();
    }else{
        Quaternion rotX = Quaternion::AxisAngle(Vector3::right, rotationXYZ.x.Get());
        Quaternion rotY = Quaternion::AxisAngle(Vector3::forward, rotationXYZ.y.Get());
        Quaternion rotZ = Quaternion::AxisAngle(Vector3::up, rotationXYZ.z.Get());

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

    tmp = 1.0f / scale.x.Get(); mat._11 *= tmp; mat._12 *= tmp; mat._13 *= tmp;
    tmp = 1.0f / scale.y.Get(); mat._21 *= tmp; mat._22 *= tmp; mat._23 *= tmp;
    tmp = 1.0f / scale.z.Get(); mat._31 *= tmp; mat._32 *= tmp; mat._33 *= tmp;

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

void Transform::SetFrame(float frame){
    position.SetFrame(frame);
    rotation.SetFrame(frame);
    rotationXYZ.SetFrame(frame);
    scale.SetFrame(frame);
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

bool AViewObject::HasAncestor(AViewObject* o){
    if (this == o)
        return true;
    return (parent == NULL ? false : parent->HasAncestor(o));
}

List<AViewObject*>& AViewObject::GetChildren(){
    return children;
}

Matrix4x4 AViewObject::GetObjectToWorldMatrix(){
    if (parent)
        return parent->GetObjectToWorldMatrix() * transform.GetMatrix();
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

void AViewObject::OnChainRender(const RenderOptions* options){
    size_t len = children.Size();

    if (parent){
        transform.chainMat = parent->transform.chainMat * transform.GetMatrix();
    }else{
        transform.chainMat = transform.GetMatrix();
    }

    transform.PushMatrix();
    for (size_t i = 0; i < len; i++)
        children[i]->OnChainRender(options);
    OnRender(options);
    transform.PopMatrix();
}

void AViewObject::OnRender(const RenderOptions* options){}

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

void AViewObject::OnAnimationFrame(float frame){
    size_t len = children.Size();
    transform.SetFrame(frame);
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
    Matrix4x4 curMat = GetWorldToObjectMatrix();

    ori = curMat * Vector4(ori, 1.0f);
    dir = curMat * Vector4(dir, 0.0f);

    switch (Main::data->selType){
    case GlobalData::SELECT_VERTICES:{
        Vertex* v = mesh->Find(ori, dir);
        if (!v){
            Main::data->selPoints.Clear();
            DebugLog("No Point Selected");
        }else{
            if (Main::data->selPoints.HasValue(v))
                return;
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
            if (Main::data->selEdges.HasValue(e))
                return;
            Main::data->selEdges.Add(e);
            DebugLog("Select Edge (%f,%f,%f) (%f,%f,%f)", e->v1->pos.x, e->v1->pos.y, e->v1->pos.z, e->v2->pos.x, e->v2->pos.y, e->v2->pos.z);
        }
    }
        break;
    }
}

void MeshObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){
    Matrix4x4 curMat = GetWorldToObjectMatrix();

    camPos = curMat * Vector4(camPos, 1.0f);
    camDir = Quaternion::FromTo(Vector3::forward, curMat * (camDir * Vector3::forward));

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
        if (Main::data->selPoints.HasValue(v))
            return;
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
    mesh->Render(options->light);
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
    for (int i = 0; i < 4; i++)
        v[i].object = this;
}

BezierCurveObject::~BezierCurveObject(){
    Free(children);
}

void BezierCurveObject::OnSelect(Vector3 ori, Vector3 dir){
    Matrix4x4 curMat = GetWorldToObjectMatrix();

    ori = curMat * Vector4(ori, 1.0f);
    dir = curMat * Vector4(dir, 0.0f);

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
void BezierCurveObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){
    Matrix4x4 curMat = GetWorldToObjectMatrix();

    camPos = curMat * Vector4(camPos, 1.0f);
    camDir = Quaternion::FromTo(Vector3::forward, curMat * (camDir * Vector3::forward));

    bool hit = false;
    for (int i = 0; i < 4; i++){
        if (v[i].Hit(camPos, camDir, zBound, p1, p2)){
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

void BezierCurveObject::OnSelectUV(Vector2 uv, float err){
    AViewObject::OnSelectUV(uv, err);
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

void BezierCurveObject::OnSelectUV(Vector2 uv1, Vector2 uv2){
    AViewObject::OnSelectUV(uv1, uv2);
    for (int i = 0; i < 4; i++){
        if (v[i].HitUV(uv1, uv2))
            Main::data->selPoints.Add(&v[i]);
    }
}

void BezierCurveObject::OnRender(const RenderOptions* options){
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
    light = GLLights::Create();
    UpdateLight();
    v.object = this;
}

PointLightObject::~PointLightObject(){
    Free(children);
    GLLights::Destroy(light);
}

void PointLightObject::OnSelect(Vector3 ori, Vector3 dir){
    Matrix4x4 curMat = GetWorldToObjectMatrix();

    ori = curMat * Vector4(ori, 1.0f);
    dir = curMat * Vector4(dir, 0.0f);

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

void PointLightObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){
    Matrix4x4 curMat = GetWorldToObjectMatrix();

    camPos = curMat * Vector4(camPos, 1.0f);
    camDir = Quaternion::FromTo(Vector3::forward, curMat * (camDir * Vector3::forward));

    if (v.Hit(camPos, camDir, zBound, p1, p2)){
        if (Main::data->selPoints.HasValue(&v))
            return;
        Main::data->selPoints.Add(&v);
        DebugLog("Select Point %f %f %f", v.pos.x, v.pos.y, v.pos.z);
    }else{
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void PointLightObject::OnRender(const RenderOptions* options){
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
    Vector3 pos = transform.chainMat * Vector4(v.pos, 1.0f);

    GLfloat position[] = {pos.x, pos.y, pos.z, 1.0};// 最后一个参数为1.0表示该光源是point light

    GLfloat ambient[] = {0.0, 0.0, 0.0, 0.0};// 暂不使用环境光
    GLfloat diffuse[] = {v.color.x, v.color.y, v.color.z, 1.0};
    GLfloat specular[] = {v.color.x, v.color.y, v.color.z, 1.0};

    glLightfv(light, GL_AMBIENT, ambient);
    glLightfv(light, GL_DIFFUSE, diffuse);
    glLightfv(light, GL_SPECULAR, specular);
    glLightfv(light, GL_POSITION, position);
}

AudioListenerObject::AudioListenerObject() : AViewObject(L"AudioListener"){
    v.object = this;
}

AudioListenerObject::~AudioListenerObject(){
    Free(children);
}

void AudioListenerObject::OnSelect(Vector3 ori, Vector3 dir){
    Matrix4x4 curMat = GetWorldToObjectMatrix();

    ori = curMat * Vector4(ori, 1.0f);
    dir = curMat * Vector4(dir, 0.0f);
    
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

void AudioListenerObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){
    Matrix4x4 curMat = GetWorldToObjectMatrix();

    camPos = curMat * Vector4(camPos, 1.0f);
    camDir = Quaternion::FromTo(Vector3::forward, curMat * (camDir * Vector3::forward));

    if (v.Hit(camPos, camDir, zBound, p1, p2)){
        if (Main::data->selPoints.HasValue(&v))
            return;
        Main::data->selPoints.Add(&v);
        DebugLog("Select Point %f %f %f", v.pos.x, v.pos.y, v.pos.z);
    }else{
        Main::data->selPoints.Clear();
        DebugLog("No Point Selected");
    }
}

void AudioListenerObject::OnRender(const RenderOptions* options){
    Main::data->audioPos = transform.chainMat * Vector4(v.pos, 1.0f);

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

const float CameraObject::SCALE = 0.3f;

CameraObject::CameraObject() : AViewObject(L"Camera"){
    pos.object = this;
    lookAt.object = this;
}

CameraObject::~CameraObject(){
    Free(children);
}

void CameraObject::OnSelect(Vector3 ori, Vector3 dir){
    Matrix4x4 curMat = GetWorldToObjectMatrix();

    ori = curMat * Vector4(ori, 1.0f);
    dir = curMat * Vector4(dir, 0.0f);
    
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

void CameraObject::OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){
    Matrix4x4 curMat = GetWorldToObjectMatrix();
    bool hit = false;

    camPos = curMat * Vector4(camPos, 1.0f);
    camDir = Quaternion::FromTo(Vector3::forward, curMat * (camDir * Vector3::forward));

    if (pos.Hit(camPos, camDir, zBound, p1, p2)){
        hit = true;
        if (Main::data->selPoints.HasValue(&pos))
            return;
        Main::data->selPoints.Add(&pos);
        DebugLog("Select Point %f %f %f", pos.pos.x, pos.pos.y, pos.pos.z);
    }
    
    if (lookAt.Hit(camPos, camDir, zBound, p1, p2)){
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

void CameraObject::OnRender(const RenderOptions* options){
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