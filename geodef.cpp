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
    pack.res = __null;
    edges.Foreach<decltype(pack)*>([](Edge* e, decltype(pack)* p){
        if (e->v1 == p->v || e->v2 == p->v){
            p->res = e;
        }
    }, &pack);
    return pack.res;
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