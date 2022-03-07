#include <utils/math3d/ViewObject.h>

#include <lib/openal/al.h>

#include <main.h>
#include <res.h>
#include <editor/MainWindow.h>
#include <editor/AudioPlayerWindow.h>
#include <utils/AudioUtils.h>
#include <utils/os/Time.h>
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

void Transform::InsertPos(float frame){
    position.InsertValue(frame);
}

void Transform::InsertRot(float frame){
    if (rotationMode == ROT_QUATERNION){
        rotation.InsertValue(frame);
    }else{
        rotationXYZ.InsertValue(frame);
    }
}

void Transform::InsertScale(float frame){
    scale.InsertValue(frame);
}

SelectInfo::SelectInfo(){}
SelectInfo::~SelectInfo(){}

bool SelectInfo::Inside(Vector3 pos) const{
    Vector3 lookPos = (-camDir) * (pos - camPos);
    if (lookPos.y < zBound.x || lookPos.y > zBound.y)
        return false;
    return rect.Inside(Vector2(lookPos.x, lookPos.z) / lookPos.y);
}

AViewObject::AViewObject() : name(L"Object") {}
AViewObject::AViewObject(ViewObjectType type) : name(L"Object"), type(type) {}
AViewObject::AViewObject(const wchar_t* name) : name(name) {}
AViewObject::AViewObject(const wchar_t* name, ViewObjectType type) : name(name), type(type) {}
AViewObject::AViewObject(WString name) : name(name){}
AViewObject::AViewObject(WString name, ViewObjectType type) : name(name), type(type) {}

AViewObject::~AViewObject(){
    Free(children);
    if (parent)
        parent->DeleteChild(this);
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

ViewObjectType AViewObject::GetType(){
    return type;
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

Matrix4x4 AViewObject::GetParentChainMat(){
    return (parent == NULL ? Matrix4x4::identity : parent->transform.chainMat);
}

Matrix4x4 AViewObject::GetParentChainInvMat(){
    return (parent == NULL ? Matrix4x4::identity : parent->transform.chainInvMat);
}

Point3 AViewObject::GetWorldPos(){
    return transform.chainMat * Point3::zero;
}

void AViewObject::SetWorldPos(Point3 pos){
    transform.position.Set(parent == NULL ? pos : parent->transform.chainInvMat * pos);
}

AViewObject* AViewObject::QueryObject(WString path){
    size_t pos = path.FindChar('.');
    WString name = path.SubString(0, pos);
    size_t len = children.Size();
    for (size_t i = 0; i < len; i++)
        if (children[i]->name == name)
            return (pos == -1 ? children[i] : children[i]->QueryObject(path.SubString(pos + 1)));
    return NULL;
}

List<AViewObject*>& AViewObject::GetChildren(){
    return children;
}

void AViewObject::OnSelect(Vector3 ori, Vector3 dir){}
void AViewObject::OnSelect(const SelectInfo* info){}
void AViewObject::OnSelectUV(Vector2 uv, float err){}
void AViewObject::OnSelectUV(Vector2 uv1, Vector2 uv2){}

Mesh* AViewObject::GetMesh(){ return NULL; }

void AViewObject::OnChainRender(const RenderOptions* options){
    size_t len = children.Size();

    if (parent){
        transform.chainMat = parent->transform.chainMat * transform.GetMatrix();
        transform.chainInvMat = transform.GetInvMatrix() * parent->transform.chainInvMat;
    }else{
        transform.chainMat = transform.GetMatrix();
        transform.chainInvMat = transform.GetInvMatrix();
    }

    transform.PushMatrix();
    for (size_t i = 0; i < len; i++)
        children[i]->OnChainRender(options);
    OnRender(options);
    transform.PopMatrix();
}

void AViewObject::OnRender(const RenderOptions* options){
    if (options->editor && Main::data->selType == GlobalData::SELECT_OBJECT && Main::data->curObject == this){
        glDisable(GL_LIGHTING);
        glEnable(GL_POINT_SMOOTH);
        glPointSize(10.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertexv3(Point3::zero);
        glEnd();
        glDisable(GL_POINT_SMOOTH);

        switch (options->objOp){
        case ObjectOperation::MOVE:
            glColor3f(1.0f, 0.0f, 0.0f);
            GLUtils::Draw3DArrow(Point3::zero, Vector3::right, 0.3f, 0.3f, 5.0f);
            glColor3f(0.0f, 1.0f, 0.0f);
            GLUtils::Draw3DArrow(Point3::zero, Vector3::forward, 0.3f, 0.3f, 5.0f);
            glColor3f(0.0f, 0.0f, 1.0f);
            GLUtils::Draw3DArrow(Point3::zero, Vector3::up, 0.3f, 0.3f, 5.0f);
            break;
        case ObjectOperation::ROTATE:
            break;
        case ObjectOperation::SCALE:
            break;
        }
    }
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

void AViewObject::OnAnimationFrame(float frame){
    size_t len = children.Size();
    transform.SetFrame(frame);
    for (size_t i = 0; i < len; i++)
        children[i]->OnAnimationFrame(frame);
}

MeshObject::MeshObject() : AViewObject(L"Mesh", ViewObjectType::OBJECT_MESH) {
    mesh = new Mesh(this);
}

MeshObject::~MeshObject(){
    // 子类析构器在返回前会自动调用父类析构器
    // 因此不要再这里调用AViewObject::~AViewObject，不然会导致多次delete使程序崩溃
    if (mesh) delete mesh;
}

void MeshObject::OnSelect(Vector3 ori, Vector3 dir){
    ori = transform.chainInvMat * Vector4(ori, 1.0f);
    dir = transform.chainInvMat * Vector4(dir, 0.0f);

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

void MeshObject::OnSelect(const SelectInfo* info){
    switch (Main::data->selType){
    case GlobalData::SELECT_VERTICES:
        mesh->FindScreenRect(info, transform.chainMat, Main::data->selPoints);
        break;
    }
}

void MeshObject::OnSelectUV(Vector2 uv, float err){
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
    mesh->FindUVRect(uv1, uv2, Main::data->selPoints);
}

Mesh* MeshObject::GetMesh(){
    return mesh;
}

void MeshObject::OnRender(const RenderOptions* options){
    AViewObject::OnRender(options);

    mesh->Render(options);
}

void MeshObject::OnRenderUVMap(){
    AViewObject::OnRenderUVMap();
    mesh->RenderUVMap();
}

BezierCurveObject::BezierCurveObject() : AViewObject(L"BezierCurve", ViewObjectType::OBJECT_BEZIER_CURVE) {
    v[0].pos = Vector3(-5.0f, 0.0f, 0.0f);
    v[1].pos = Vector3(0.0f, 0.0f, 0.0f);
    v[2].pos = Vector3(0.0f, 0.0f, 5.0f);
    v[3].pos = Vector3(5.0f, 0.0f, 5.0f);
    for (int i = 0; i < 4; i++)
        v[i].object = this;
}

BezierCurveObject::~BezierCurveObject(){}

void BezierCurveObject::OnSelect(Vector3 ori, Vector3 dir){
    ori = transform.chainInvMat * Vector4(ori, 1.0f);
    dir = transform.chainInvMat * Vector4(dir, 0.0f);

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
void BezierCurveObject::OnSelect(const SelectInfo* info){
    bool hit = false;
    for (int i = 0; i < 4; i++){
        if (info->Inside(transform.chainMat * Vector4(v[i].pos, 1.0f))){
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

PointLightObject::PointLightObject() : AViewObject(L"PointLight", ViewObjectType::OBJECT_POINT_LIGHT) {
    light = GLLights::Create();
    UpdateLight();
    v.object = this;
}

PointLightObject::~PointLightObject(){
    GLLights::Destroy(light);
}

void PointLightObject::OnSelect(Vector3 ori, Vector3 dir){
    ori = transform.chainInvMat * Vector4(ori, 1.0f);
    dir = transform.chainInvMat * Vector4(dir, 0.0f);

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
    if (info->Inside(transform.chainMat * Vector4(v.pos, 1.0f))){
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

int AudioSourceObject::REPLAY_ERROR = 4000;

AudioSourceObject::AudioSourceObject(uenum format, char* data, int size, int freq) : AViewObject(L"AudioSource", ViewObjectType::OBJECT_AUDIO_SOURCE) {
    alGenSources(1, &alSrc);
    alGenBuffers(1, &alBuf);

    alSourcef(alSrc, AL_REFERENCE_DISTANCE, 1.0f);
    alSourcef(alSrc, AL_ROLLOFF_FACTOR, 2.0f);
    alSourcef(alSrc, AL_MAX_DISTANCE, 100.0f);

    alBufferData(alBuf, format, data, size, freq);
    alSourceQueueBuffers(alSrc, 1, &alBuf);

    switch (format){
    case AL_FORMAT_MONO8:
        alSampleSize = 1;
        alChannels = 1;
        break;
    case AL_FORMAT_MONO16:
        alSampleSize = 2;
        alChannels = 1;
        break;
    case AL_FORMAT_STEREO8:
        alSampleSize = 2;
        alChannels = 2;
        break;
    case AL_FORMAT_STEREO16:
        alSampleSize = 4;
        alChannels = 2;
        break;
    }

    alAudioData = data;
    alAudioSize = size / alSampleSize;
    alAudioLen = alAudioSize / freq;
    alAudioFreq = freq;
    alAudioOffset = 0;

    recPos = Vector3::zero;
    recTime = TimeUtils::GetTime();
}

AudioSourceObject::~AudioSourceObject(){
    if (alAudioData) delete[] (char*)alAudioData;

    alDeleteSources(1, &alSrc);
    alDeleteBuffers(1, &alBuf);

    if (window) window->LoadObject(NULL);
}

void AudioSourceObject::OnTimer(int id){
    Point3 pos = transform.chainMat * Point3::zero;
    if (dopplerEffect){
        SetPosAutoVelv3(pos);
    }else{
        SetPosv3(pos);
    }
}

void AudioSourceObject::OnRender(const RenderOptions* options){
    AViewObject::OnRender(options);

    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(4.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    glDisable(GL_POINT_SMOOTH);
}

char* AudioSourceObject::GetData(){
    return (char*)alAudioData;
}

int AudioSourceObject::GetSize(){
    return alAudioSize;
}

int AudioSourceObject::GetLength(){
    return alAudioLen;
}

int AudioSourceObject::GetSampleSize(){
    return alSampleSize;
}

int AudioSourceObject::GetChannelCount(){
    return alChannels;
}

int AudioSourceObject::GetFrequency(){
    return alAudioFreq;
}

int AudioSourceObject::GetOffset(){
    if (IsPlaying())
        alGetSourcei(alSrc, AL_SAMPLE_OFFSET, &alAudioOffset);
    return alAudioOffset;
}

bool AudioSourceObject::IsLoop(){
    ALint loop;
    alGetSourcei(alSrc, AL_LOOPING, &loop);
    return loop == AL_TRUE;
}

float AudioSourceObject::GetGain(){
    float gain;
    alGetSourcef(alSrc, AL_GAIN, &gain);
    return gain;
}

AudioPlayerWindow* AudioSourceObject::GetWindowRef(){
    return window;
}

void AudioSourceObject::SetOffset(int offset){
    if (IsPlaying())
        alSourcei(alSrc, AL_SAMPLE_OFFSET, offset);
    alAudioOffset = offset;
}

void AudioSourceObject::SetLoop(bool loop){
    alSourcei(alSrc, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void AudioSourceObject::SetGain(float gain){
    alSourcef(alSrc, AL_GAIN, gain);
}

void AudioSourceObject::SetWindowRef(AudioPlayerWindow* window){
    this->window = window;
}

void AudioSourceObject::Play(){
    alSourcePlay(alSrc);
    if (alAudioOffset + REPLAY_ERROR >= alAudioSize)
        alAudioOffset = 0;
    alSourcei(alSrc, AL_SAMPLE_OFFSET, alAudioOffset);
}

void AudioSourceObject::Stop(){
    alGetSourcei(alSrc, AL_SAMPLE_OFFSET, &alAudioOffset);
    alSourceStop(alSrc);
}

bool AudioSourceObject::IsPlaying(){
    ALint state;
    alGetSourcei(alSrc, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void AudioSourceObject::SetPosv3(Point3 value){
    alSource3f(alSrc, AL_POSITION, value.x, value.y, value.z);
}

void AudioSourceObject::SetVelocityv3(Vector3 value){
    alSource3f(alSrc, AL_VELOCITY, value.x, value.y, value.z);
}

void AudioSourceObject::SetPosAutoVelv3(Point3 value){
    float curTime = TimeUtils::GetTime();
    Vector3 vel = (value - recPos) / (curTime - recTime);

    recPos = value;
    recTime = curTime;

    alSource3f(alSrc, AL_POSITION, value.x, value.y, value.z);
    alSource3f(alSrc, AL_VELOCITY, vel.x, vel.y, vel.z);
}

void AudioSourceObject::SetDopplerEffect(bool on){
    if (dopplerEffect == on)
        return;
    dopplerEffect = on;
    if (on){
        recPos = transform.chainMat * Point3::zero;
        recTime = TimeUtils::GetTime();
    }else{
        SetVelocityv3(Vector3::zero);
    }
}

bool AudioSourceObject::HasDopplerEffect(){
    return dopplerEffect;
}

AudioListenerObject::AudioListenerObject() : AViewObject(L"AudioListener", ViewObjectType::OBJECT_AUDIO_LISTENER) {
    recPos = Vector3::zero;
    recTime = TimeUtils::GetTime();
}

AudioListenerObject::~AudioListenerObject(){
    LocalData::GetLocalInst()->audioListener = NULL;
}

void AudioListenerObject::OnRender(const RenderOptions* options){
    AViewObject::OnRender(options);

    Vector3 pos = transform.chainMat * Vector4(0.0f, 0.0f, 0.0f, 1.0f);
    Vector3 dir = (transform.chainMat * Vector4(Vector3::forward, 0.0f)).Normal();
    Vector3 up = (transform.chainMat * Vector4(Vector3::up, 0.0f)).Normal();

    alListenerPosv3(pos);
    alListenerDirv3(dir, up);

    glColor3f(1.0f, 0.0f, 0.0f);
    GLUtils::Draw3DArrow(Point3::zero, Vector3::forward, 0.3f, 0.3f, 10.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    GLUtils::Draw3DArrow(Point3::zero, Vector3::up * 0.5f, 0.08f, 0.3f, 5.0f);
}

void AudioListenerObject::SetPosv3(Point3 value){
    alListener3f(AL_POSITION, value.x, value.y, value.z);
}

void AudioListenerObject::SetVelocityv3(Vector3 value){
    alListener3f(AL_VELOCITY, value.x, value.y, value.z);
}

void AudioListenerObject::SetPosAutoVelv3(Point3 value){
    float curTime = TimeUtils::GetTime();
    Vector3 vel = (value - recPos) / (curTime - recTime);

    recPos = value;
    recTime = curTime;

    alListener3f(AL_POSITION, value.x, value.y, value.z);
    alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
}

void AudioListenerObject::SetDopplerEffect(bool on){
    if (dopplerEffect == on)
        return;
    dopplerEffect = on;
    if (on){
        recPos = transform.chainMat * Point3::zero;
        recTime = TimeUtils::GetTime();
    }else{
        SetVelocityv3(Vector3::zero);
    }
}

bool AudioListenerObject::HasDopplerEffect(){
    return dopplerEffect;
}

const float CameraObject::SCALE = 0.3f;

CameraObject::CameraObject() : AViewObject(L"Camera", ViewObjectType::OBJECT_CAMERA) {
    pos.object = this;
    lookAt.object = this;
}

CameraObject::~CameraObject(){
    LocalData::GetLocalInst()->camera = NULL;
}

void CameraObject::OnSelect(Vector3 ori, Vector3 dir){
    ori = transform.chainInvMat * Vector4(ori, 1.0f);
    dir = transform.chainInvMat * Vector4(dir, 0.0f);
    
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

    if (info->Inside(transform.chainMat * Vector4(pos.pos, 1.0f))){
        hit = true;
        if (Main::data->selPoints.HasValue(&pos))
            return;
        Main::data->selPoints.Add(&pos);
        DebugLog("Select Point %f %f %f", pos.pos.x, pos.pos.y, pos.pos.z);
    }
    
    if (info->Inside(transform.chainMat * Vector4(lookAt.pos, 1.0f))){
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
    AViewObject::OnRender(options);

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