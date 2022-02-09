#include "geodef.h"

#include "log.h"

Vertex::Vertex(){}

Vertex::Vertex(Vector3 pos) : pos(pos) {}

Vertex::~Vertex(){}

void Vertex::UpdateNormal(){
    Vector3 dir = Vector3::zero;
    faces.Foreach<Vector3*>([](Face* f, Vector3* n){
        *n += f->normal;
    }, &dir);
    if (dir.SqrMagnitude() == 0.0f){
        normal = Vector3::forward;
        return;
    }
    normal = dir.Normal();
}

void Vertex::AddEdge(Edge* e){
    //DebugLog("Vertex::AddEdge %p", e);
    edges.Add(e);
}

void Vertex::AddFace(Face* f){
    //DebugLog("Vertex::AddFace %p", f);
    faces.Add(f);
}

bool Vertex::FindEdge(Edge* e){
    return edges.HasValue(e);
}

bool Vertex::FindFace(Face* f){
    return faces.HasValue(f);
}

bool Vertex::DeleteEdge(Edge* e){
    return edges.Remove(e);
}

bool Vertex::DeleteFace(Face* f){
    return faces.Remove(f);
}

Edge* Vertex::EdgeRelateTo(Vertex* v){
    struct {
        Vertex* v;
        Edge* res;
    } pack;
    pack.v = v;
    pack.res = NULL;
    edges.Foreach<decltype(pack)*>([](Edge* e, decltype(pack)* p){
        if (e->v1 == p->v || e->v2 == p->v){
            p->res = e;
        }
    }, &pack);
    return pack.res;
}

bool Vertex::Hit(Vector3 ori, Vector3 dir){
    return Vector3::Cosine(dir, pos - ori) > 0.9997f;
}

bool Vertex::Hit(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2){
    Vector3 lookPos = (-camDir) * (pos - camPos);
    if (lookPos.y < zBound.x || lookPos.y > zBound.y){
        return false;
    }
    float inv = 1.0f / lookPos.y;
    float x = lookPos.x * inv;
    float z = lookPos.z * inv;
    Sort(p1.x, p2.x); Sort(p1.y, p2.y);
    return x >= p1.x && x <= p2.x && z >= p1.y && z <= p2.y;
}

bool Vertex::HitUV(Vector2 uv, float err){
    return (this->uv - uv).SqrMagnitude() <= err * err;
}

bool Vertex::HitUV(Vector2 uv1, Vector2 uv2){
    Sort(uv1.x, uv2.x); Sort(uv1.y, uv2.y);
    return uv.x >= uv1.x && uv.x <= uv2.x && uv.y >= uv1.y && uv.y <= uv2.y;
}

Edge::Edge(){}

Edge::Edge(Vertex* v1, Vertex* v2) : v1(v1), v2(v2) {
    v1->AddEdge(this);
    v2->AddEdge(this);
}

Edge::~Edge(){}

void Edge::AddFace(Face* f){
    faces.Add(f);
}

bool Edge::FindFace(Face* f){
    return faces.HasValue(f);
}

bool Edge::DeleteFace(Face* f){
    return faces.Remove(f);
}

void Edge::DeleteSelfReference(){
    if(v1) v1->DeleteEdge(this);
    if(v2) v2->DeleteEdge(this);
    faces.Foreach<Edge*>([](Face* f, Edge* e){
        f->DeleteEdge(e);
    }, this);
}

void Edge::DeleteSelfReferenceExcept(Vertex* v){
    if(v1 && v1 != v) v1->DeleteEdge(this);
    if(v2 && v2 != v) v2->DeleteEdge(this);
    faces.Foreach<Edge*>([](Face* f, Edge* e){
        f->DeleteEdge(e);
    }, this);
}

Face::Face(){}

Face::~Face(){}

Face* Edge::FaceRelateTo(Vertex* v){
    struct {
        Vertex* v;
        Face* res;
    } pack;
    pack.v = v;
    pack.res = __null;
    faces.Foreach<decltype(pack)*>([](Face* f, decltype(pack)* p){
        if (f->FindVertex(p->v)){
            p->res = f;
        }
    }, &pack);
    return pack.res;
}

void Face::AddVertex(Vertex* v){
    vertices.Add(v);
}

void Face::AddEdge(Edge* e){
    edges.Add(e);
}

bool Face::FindVertex(Vertex* v){
    return vertices.HasValue(v);
}

bool Face::FindEdge(Edge* e){
    return edges.HasValue(e);
}

bool Face::DeleteVertex(Vertex* v){
    return vertices.Remove(v);
}

bool Face::DeleteEdge(Edge* e){
    return edges.Remove(e);
}

void Face::DeleteSelfReference(){
    vertices.Foreach<Face*>([](Vertex* v, Face* f){
        v->DeleteFace(f);
    }, this);
    edges.Foreach<Face*>([](Edge* e, Face* f){
        e->DeleteFace(f);
    }, this);
}

void Face::DeleteSelfReferenceExcept(Vertex* v){
    struct {
        Vertex* v;
        Face* f;
    } pack;
    pack.f = this;
    pack.v = v;
    vertices.Foreach<decltype(pack)*>([](Vertex* v, decltype(pack)* p){
        if(v != p->v) v->DeleteFace(p->f);
    }, &pack);
    edges.Foreach<Face*>([](Edge* e, Face* f){
        e->DeleteFace(f);
    }, this);
}