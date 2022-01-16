#include "mesh.h"

#include <gl/gl.h>

#include "log.h"

Mesh::Mesh(){}

Mesh::~Mesh(){
    Free(vertices);
    Free(edges);
    Free(faces);
}

Vertex* Mesh::Find(Vector3 ori, Vector3 dir){
    struct {
        float selectDis;
        Vector3 start;
        Vector3 forward;
        Vertex* res;
    }pack;
    pack.selectDis = -1.0f;
    pack.start = ori;
    pack.forward = dir;
    vertices.Foreach<decltype(pack)*>([](Vertex* p, decltype(pack)* m){
        if (Vector3::Cosine(m->forward, p->pos - m->start) > 0.9997f){
            if (m->selectDis < 0.0f || (p->pos - m->start).SqrMagnitude() < m->selectDis * m->selectDis){
                m->selectDis = (p->pos - m->start).Magnitude();
                m->res = p;
            }
        }
    }, &pack);
    if (pack.selectDis < 0.0f){
        return NULL;
    }else{
        return pack.res;
    }
}

Vertex* Mesh::AddVertex(Vector3 pos){
    Vertex* v = new Vertex();
    v->pos = pos;
    vertices.Add(v);
    return v;
}

Edge* Mesh::AddEdge(Vertex* v1, Vertex* v2){
    DebugLog("Mesh::AddEdge %p %p", v1, v2);
    // 边去重
    struct {
        bool has;
        Vertex* v1;
        Vertex* v2;
        Edge* e;
    } pack;
    pack.has = false;
    pack.v1 = v1;
    pack.v2 = v2;
    v1->edges.Foreach<decltype(pack)*>([](Edge* e, decltype(pack)* res){
        if (e->v1 == res->v1 && e->v2 == res->v2){
            res->has = true;
            res->e = e;
        }else if (e->v1 == res->v2 && e->v2 == res->v1){
            res->has = true;
            res->e = e;
        }
    }, &pack);
    if (pack.has){
        return pack.e;
    }
    Edge* e = new Edge();
    e->v1 = v1;
    e->v2 = v2;
    v1->edges.Add(e);
    v2->edges.Add(e);
    edges.Add(e);
    return e;
}

Face* Mesh::AddTriFace(Vertex* v1, Vertex* v2, Vertex* v3){
    DebugLog("Mesh::AddTriFace %p %p %p", v1, v2, v3);
    Edge* e1 = AddEdge(v1, v2);
    Edge* e2 = AddEdge(v2, v3);
    Edge* e3 = AddEdge(v3, v1);
    // TODO 面元去重未完成
    Face* f = new Face();
    f->vertices.Add(v1);
    f->vertices.Add(v2);
    f->vertices.Add(v3);
    f->edges.Add(e1);
    f->edges.Add(e2);
    f->edges.Add(e3);
    f->normal = Vector3::Cross(v1->pos - v2->pos, v1->pos - v3->pos).Normal();
    v1->faces.Add(f); v1->UpdateNormal();
    v2->faces.Add(f); v2->UpdateNormal();
    v3->faces.Add(f); v3->UpdateNormal();
    e1->AddFace(f);
    e2->AddFace(f);
    e3->AddFace(f);
    faces.Add(f);
    return NULL;
}

void Mesh::DeleteVertex(Vertex* v){
    DebugLog("Mesh::DeleteVertex %p", v);
}

void Mesh::DeleteEdge(Edge* e){
    DebugLog("Mesh::DeleteEdge %p", e);
}

void Mesh::DeleteTriFace(Face* f){
    DebugLog("Mesh::DeleteTriFace %p", f);
}

void Mesh::Render(){
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(4.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    vertices.Foreach([](Vertex* v){
        //glColor3f(v->color.x, v->color.y, v->color.z);
        glVertex3f(v->pos.x, v->pos.y, v->pos.z);
    });
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    edges.Foreach([](Edge* e){
        //glColor3f(e->v1->color.x, e->v1->color.y, e->v1->color.z);
        glVertex3f(e->v1->pos.x, e->v1->pos.y, e->v1->pos.z);
        //glColor3f(e->v2->color.x, e->v2->color.y, e->v2->color.z);
        glVertex3f(e->v2->pos.x, e->v2->pos.y, e->v2->pos.z);
    });
    glEnd();
    glDisable(GL_LINE_SMOOTH);

    glDisable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    faces.Foreach([](Face* f){
        Vertex* v;
        v = f->vertices.GetItem(0);
        glColor3f(v->color.x, v->color.y, v->color.z); glNormal3f(v->normal.x, v->normal.y, v->normal.z); glVertex3f(v->pos.x, v->pos.y, v->pos.z);
        v = f->vertices.GetItem(1);
        glColor3f(v->color.x, v->color.y, v->color.z); glNormal3f(v->normal.x, v->normal.y, v->normal.z); glVertex3f(v->pos.x, v->pos.y, v->pos.z);
        v = f->vertices.GetItem(2);
        glColor3f(v->color.x, v->color.y, v->color.z); glNormal3f(v->normal.x, v->normal.y, v->normal.z); glVertex3f(v->pos.x, v->pos.y, v->pos.z);
    });
    glEnd();
}