#include "geodef.h"

#include "log.h"

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

void Edge::AddFace(Face* f){
    if (f1 == NULL){
        f1 = f;
    }else{
        f2 = f;
    }
}