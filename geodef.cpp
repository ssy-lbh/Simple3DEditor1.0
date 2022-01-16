#include "geodef.h"

void Vertex::UpdateNormal(){
    Vector3 dir = Vector3::zero;
    faces.Foreach<Vector3*>([](Face* f, Vector3* n){
        *n += f->normal;
    }, &dir);
    normal = dir.Normal();
}

void Edge::AddFace(Face* f){
    if (f1 == NULL){
        f1 = f;
    }else{
        f2 = f;
    }
}