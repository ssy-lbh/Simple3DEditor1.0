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
    Vertex* v = new Vertex(pos);
    vertices.Add(v);
    return v;
}

Vertex* Mesh::AddVertex(Vertex* v){
    if (vertices.HasValue(v)){
        return v;
    }
    vertices.Add(v);
    return v;
}

Edge* Mesh::AddEdge(Vertex* v1, Vertex* v2){
    //DebugLog("Mesh::AddEdge %p %p", v1, v2);
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
    AddVertex(v1);
    AddVertex(v2);
    Edge* e = new Edge(v1, v2);
    edges.Add(e);
    return e;
}

Face* Mesh::AddTriFace(Vertex* v1, Vertex* v2, Vertex* v3){
    //DebugLog("Mesh::AddTriFace %p %p %p", v1, v2, v3);
    Edge* e1 = AddEdge(v1, v2);
    Edge* e2 = AddEdge(v2, v3);
    Edge* e3 = AddEdge(v3, v1);
    Face* f = e1->FaceRelateTo(v3);
    if (f){
        return f;
    }
    f = new Face();
    f->vertices.Clear();
    f->vertices.Add(v1);
    f->vertices.Add(v2);
    f->vertices.Add(v3);
    f->edges.Clear();
    f->edges.Add(e1);
    f->edges.Add(e2);
    f->edges.Add(e3);
    f->normal = Vector3::Cross(v1->pos - v2->pos, v1->pos - v3->pos).Normal();
    v1->AddFace(f); v1->UpdateNormal();
    v2->AddFace(f); v2->UpdateNormal();
    v3->AddFace(f); v3->UpdateNormal();
    e1->AddFace(f);
    e2->AddFace(f);
    e3->AddFace(f);
    faces.Add(f);
    return NULL;
}

void Mesh::DeleteVertex(Vertex* v){
    DebugLog("Mesh::DeleteVertex %p", v);
    struct {
        Mesh* m;
        Vertex* v;
    } pack;
    pack.m = this;
    pack.v = v;
    v->faces.Foreach<decltype(pack)*>([](Face* f, decltype(pack)* p){
        f->DeleteSelfReferenceExcept(p->v);
        p->m->faces.Remove(f);
        delete f;
    }, &pack);
    v->edges.Foreach<decltype(pack)*>([](Edge* e, decltype(pack)* p){
        e->DeleteSelfReferenceExcept(p->v);
        p->m->edges.Remove(e);
        delete e;
    }, &pack);
    vertices.Remove(v);
    delete v;
}

void Mesh::DeleteEdge(Edge* e){
    DebugLog("Mesh::DeleteEdge %p [Unimplemented]", e);
}

void Mesh::DeleteTriFace(Face* f){
    DebugLog("Mesh::DeleteTriFace %p [Unimplemented]", f);
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

void Mesh::WriteToOBJ(HANDLE hFile){
    size_t index = 0;
    WriteFile(hFile, "# StereoVision 3D Editor\n", 25, NULL, NULL);
    WriteFile(hFile, "# Author: lin-boheng@gitee.com\n", 31, NULL, NULL);
    vertices.Foreach<size_t*>([](Vertex* v, size_t* i){
        v->index = ++*i;
    }, &index);
    vertices.Foreach<HANDLE>([](Vertex* v, HANDLE hFile){
        char tmp[MAX_PATH + 1];
        DWORD len;
        len = __builtin_snprintf(tmp, MAX_PATH, "v %f %f %f\n", v->pos.x, v->pos.y, v->pos.z);
        WriteFile(hFile, tmp, len, &len, NULL);
    }, hFile);
    // 纹理UV坐标
    // vertices.Foreach<HANDLE>([](Vertex* v, HANDLE hFile){
    //     char tmp[MAX_PATH + 1];
    //     DWORD len;
    //     len = __builtin_snprintf(tmp, MAX_PATH, "vt %f %f %f\n", v->uv.x, v->uv.y, v->uv.z);
    //     WriteFile(hFile, tmp, len, &len, NULL);
    // }, hFile);
    // 顶点法线
    // vertices.Foreach<HANDLE>([](Vertex* v, HANDLE hFile){
    //     char tmp[MAX_PATH + 1];
    //     DWORD len;
    //     len = __builtin_snprintf(tmp, MAX_PATH, "vn %f %f %f\n", v->normal.x, v->normal.y, v->normal.z);
    //     WriteFile(hFile, tmp, len, &len, NULL);
    // }, hFile);
    faces.Foreach<HANDLE>([](Face* f, HANDLE hFile){
        char tmp[MAX_PATH + 1];
        DWORD len;
        len = __builtin_snprintf(tmp, MAX_PATH, "f %d %d %d\n", f->vertices.GetItem(0)->index, f->vertices.GetItem(1)->index, f->vertices.GetItem(2)->index);
        WriteFile(hFile, tmp, len, &len, NULL);
    }, hFile);
}

size_t Mesh::FindScreenRect(Vector3 camPos, Quaternion camDir, float zNear, float zFar, float x1, float x2, float y1, float y2, List<Vertex*>& result){
    struct {
        size_t cnt;
        Vector3 camPos;
        Quaternion camDir;
        float zNear;
        float zFar;
        float x1, x2, y1, y2;
        List<Vertex*>* list;
    }pack;
    pack.cnt = 0;
    pack.camPos = camPos;
    pack.camDir = camDir;
    pack.zNear = zNear;
    pack.zFar = zFar;
    if (x1 < x2){
        pack.x1 = x1;
        pack.x2 = x2;
    }else{
        pack.x1 = x2;
        pack.x2 = x1;
    }
    if (y1 < y2){
        pack.y1 = y1;
        pack.y2 = y2;
    }else{
        pack.y1 = y2;
        pack.y2 = y1;
    }
    pack.list = &result;
    vertices.Foreach<decltype(pack)*>([](Vertex* p, decltype(pack)* m){
        Vector3 lookPos = (-m->camDir) * (p->pos - m->camPos);
        if (lookPos.y < m->zNear || lookPos.y > m->zFar){
            return;
        }
        float x = lookPos.x / lookPos.y;
        float z = lookPos.z / lookPos.y;
        if (x >= m->x1 && x <= m->x2 && z >= m->y1 && z <= m->y2){
            m->cnt++;
            m->list->Add(p);
        }
    }, &pack);
    return pack.cnt;
}