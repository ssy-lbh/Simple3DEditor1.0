#include <util/math3d/Geometry.h>

#include <util/os/Log.h>
#include <util/gl/GLUtils.h>
#include <util/math3d/Math.h>
#include <util/math3d/LinearAlgebra.h>
#include <editor/main/ViewObject.h>

namespace simple3deditor {

Vertex::Vertex(){}

Vertex::Vertex(Point3 pos) : pos(pos) {}

Vertex::~Vertex(){}

Point3 Vertex::GetWorldPos(){
    if (!object)
        return pos;
    
    return object->transform.chainMat * pos;
}

void Vertex::SetWorldPos(Point3 pos){
    if (!object){
        this->pos = pos;
        return;
    }

    this->pos = object->transform.chainInvMat * pos;
}

void Vertex::UpdateNormal(){
    Vector3 dir = Vector3::zero;
    faces.Foreach([&](Face* f){
        dir += f->normal;
    });
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
    Edge* res = NULL;

    edges.Foreach([=, &res](Edge* e){
        if (e->v1 == v || e->v2 == v){
            res = e;
        }
    });
    return res;
}

bool Vertex::Hit(Vector3 pos, Vector3 camPos, Quaternion camDir, Vector2 zBound, Rect rect){
    Vector3 lookPos = (-camDir) * (pos - camPos);
    if (lookPos.y < zBound.x || lookPos.y > zBound.y)
        return false;
    return rect.Inside(Vector2(lookPos.x, lookPos.z) / lookPos.y);
}

bool Vertex::Hit(Vector3 ori, Vector3 dir){
    return Vector3::Cosine(dir, pos - ori) > 0.9997f;
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
    faces.Foreach([=](Face* f){
        f->DeleteEdge(this);
    });
}

void Edge::DeleteSelfReferenceExcept(Vertex* v){
    if(v1 && v1 != v) v1->DeleteEdge(this);
    if(v2 && v2 != v) v2->DeleteEdge(this);
    faces.Foreach([=](Face* f){
        f->DeleteEdge(this);
    });
}

bool Edge::Hit(Vector3 ori, Vector3 dir){
    Vector3 v1 = (this->v1->pos - ori);
    Vector3 v2 = (this->v2->pos - this->v1->pos);
    Vector3 v3 = Vector3::Cross(v1, v2);
    Vector3 res = Vector3::Decompose(dir, v1, v2, v3);
    res /= res.x;
    return res.y >= 0.0f && res.y <= 1.0f && Vector3::Cosine(v1, v1 + v3 * res.z) > 0.9997f;
}

Face::Face(){}

Face::~Face(){}

Face* Edge::FaceRelateTo(Vertex* v){
    Face* res = NULL;

    faces.Foreach([=, &res](Face* f){
        if (f->FindVertex(v))
            res = f;
    });
    return res;
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
    vertices.Foreach([=](Vertex* v){
        v->DeleteFace(this);
    });
    edges.Foreach([=](Edge* e){
        e->DeleteFace(this);
    });
}

void Face::DeleteSelfReferenceExcept(Vertex* v){
    vertices.Foreach([=](Vertex* vert){
        if(vert != v) vert->DeleteFace(this);
    });
    edges.Foreach([=](Edge* e){
        e->DeleteFace(this);
    });
}

void Face::DeleteSelfReferenceExcept(Edge* e){
    vertices.Foreach([=](Vertex* v){
        v->DeleteFace(this);
    });
    edges.Foreach([=](Edge* edge){
        if (edge != e) edge->DeleteFace(this);
    });
}

bool Face::Hit(Point3 ori, Vector3 dir){
    return Intersect(ori, dir, NULL, NULL);
}

bool Face::Intersect(Point3 ori, Vector3 dir, Vector3* bary, float* dis){
    if (vertices.Size() != 3){
        DebugError("Face::Intersect Only Triangle Permitted");
        return false;
    }
    Vector3 v0 = vertices[0]->pos - ori;
    Vector3 v1 = vertices[1]->pos - vertices[0]->pos;
    Vector3 v2 = vertices[2]->pos - vertices[0]->pos;
    float det = Vector3::Determinant(v0, v1, v2);
    if (det == 0.0f)
        return false;
    Vector3 pos = Vector3::Decompose(dir, v0, v1, v2, det);
    if (pos.x <= 0.0f)
        return false;
    float invX = 1.0f / pos.x;
    pos.y *= invX; pos.z *= invX;
    if (bary)
        *bary = Vector3(1.0f - pos.y - pos.z, pos.y, pos.z);
    if (dis)
        *dis = dir.Magnitude() * invX;
    return pos.y >= 0.0f && pos.y <= 1.0f &&
            pos.z >= 0.0f && pos.z <= 1.0f;
}

}