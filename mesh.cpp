#include "mesh.h"

#include "opengl/gl/gl.h"

#include "main.h"
#include "res.h"
#include "log.h"

Mesh::Mesh(){}

Mesh::Mesh(AViewObject* object) : object(object) {}

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

Edge* Mesh::FindEdge(Vector3 ori, Vector3 dir){
    struct {
        Vector3 start;
        Vector3 forward;
        Edge* res;
    }pack;
    pack.start = ori;
    pack.forward = dir;
    pack.res = NULL;
    edges.Foreach<decltype(pack)*>([](Edge* e, decltype(pack)* m){
        if (e->Hit(m->start, m->forward)){
            m->res = e;
        }
    }, &pack);
    return pack.res;
}

Vertex* Mesh::FindUV(Vector2 uv, float err){
    struct {
        float err;
        Vector2 uv;
        Vertex* res;
    }pack;
    pack.err = err * err;
    pack.uv = uv;
    pack.res = NULL;
    vertices.Foreach<decltype(pack)*>([](Vertex* p, decltype(pack)* m){
        if (m->res)
            return;
        if ((p->uv - m->uv).SqrMagnitude() <= m->err){
            m->res = p;
        }
    }, &pack);
    return pack.res;
}

size_t Mesh::FindUVRect(Vector2 uv1, Vector2 uv2, List<Vertex*>& result){
    struct {
        size_t cnt;
        float x1, x2, y1, y2;
        List<Vertex*>* list;
    }pack;
    pack.cnt = 0;
    if (uv1.x < uv2.x){
        pack.x1 = uv1.x;
        pack.x2 = uv2.x;
    }else{
        pack.x1 = uv2.x;
        pack.x2 = uv1.x;
    }
    if (uv1.y < uv2.y){
        pack.y1 = uv1.y;
        pack.y2 = uv2.y;
    }else{
        pack.y1 = uv2.y;
        pack.y2 = uv1.y;
    }
    pack.list = &result;
    vertices.Foreach<decltype(pack)*>([](Vertex* p, decltype(pack)* m){
        if (p->uv.x >= m->x1 && p->uv.x <= m->x2 && p->uv.y >= m->y1 && p->uv.y <= m->y2){
            m->cnt++;
            m->list->Add(p);
        }
    }, &pack);
    return pack.cnt;
}

Vertex* Mesh::AddVertex(Vector3 pos){
    Vertex* v = new Vertex(pos);
    v->object = object;
    vertices.Add(v);
    return v;
}

Vertex* Mesh::AddVertex(Vertex* v){
    if (vertices.HasValue(v)){
        return v;
    }
    v->object = object;
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
    e->object = object;
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
    f->object = object;
    faces.Add(f);
    return NULL;
}

void Mesh::DeleteVertex(Vertex* v){
    //DebugLog("Mesh::DeleteVertex %p", v);
    struct {
        Mesh* m;
        Vertex* v;
    } pack;
    pack.m = this;
    pack.v = v;
    v->faces.Foreach<decltype(pack)*>([](Face* f, decltype(pack)* p){
        //DebugLog("Face::DeleteSelfReferenceExcept %p", f);
        f->DeleteSelfReferenceExcept(p->v);
        p->m->faces.Remove(f);
        delete f;
    }, &pack);
    v->edges.Foreach<decltype(pack)*>([](Edge* e, decltype(pack)* p){
        //DebugLog("Edge::DeleteSelfReferenceExcept %p", e);
        e->DeleteSelfReferenceExcept(p->v);
        p->m->edges.Remove(e);
        delete e;
    }, &pack);
    vertices.Remove(v);
    delete v;
}

void Mesh::DeleteEdge(Edge* e){
    struct {
        Mesh* m;
        Edge* e;
    } pack;
    pack.m = this;
    pack.e = e;
    e->faces.Foreach<decltype(pack)*>([](Face* f, decltype(pack)* p){
        f->DeleteSelfReferenceExcept(p->e);
        p->m->faces.Remove(f);
        delete f;
    }, &pack);
    e->faces.Clear();
    e->DeleteSelfReference();
    edges.Remove(e);
    delete e;
}

void Mesh::DeleteTriFace(Face* f){
    f->DeleteSelfReference();
    faces.Remove(f);
    delete f;
}

void Mesh::Render(){
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(4.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    vertices.Foreach([](Vertex* v){
        glVertex3f(v->pos.x, v->pos.y, v->pos.z);
    });
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    edges.Foreach([](Edge* e){
        glVertex3f(e->v1->pos.x, e->v1->pos.y, e->v1->pos.z);
        glVertex3f(e->v2->pos.x, e->v2->pos.y, e->v2->pos.z);
    });
    glEnd();
    glDisable(GL_LINE_SMOOTH);

    glDisable(GL_LIGHTING);
    if (Main::data->lightEnabled)
        glEnable(GL_LIGHTING);// 开启光照系统
    glShadeModel(GL_SMOOTH);
    if (modeltex){
        modeltex->Enable();
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        faces.Foreach([](Face* f){
            Vertex* v;
            // V坐标已经反转
            v = f->vertices[0];
            glTexCoord2f(v->uv.x, 1.0f - v->uv.y); glColor3f(v->color.x, v->color.y, v->color.z); glNormal3f(v->normal.x, v->normal.y, v->normal.z); glVertex3f(v->pos.x, v->pos.y, v->pos.z);
            v = f->vertices[1];
            glTexCoord2f(v->uv.x, 1.0f - v->uv.y); glColor3f(v->color.x, v->color.y, v->color.z); glNormal3f(v->normal.x, v->normal.y, v->normal.z); glVertex3f(v->pos.x, v->pos.y, v->pos.z);
            v = f->vertices[2];
            glTexCoord2f(v->uv.x, 1.0f - v->uv.y); glColor3f(v->color.x, v->color.y, v->color.z); glNormal3f(v->normal.x, v->normal.y, v->normal.z); glVertex3f(v->pos.x, v->pos.y, v->pos.z);
        });
        glEnd();
        glShadeModel(GL_FLAT);
        modeltex->Disable();
    }else{
        glBegin(GL_TRIANGLES);
        faces.Foreach([](Face* f){
            Vertex* v;
            v = f->vertices[0];
            glColor3f(v->color.x, v->color.y, v->color.z); glNormal3f(v->normal.x, v->normal.y, v->normal.z); glVertex3f(v->pos.x, v->pos.y, v->pos.z);
            v = f->vertices[1];
            glColor3f(v->color.x, v->color.y, v->color.z); glNormal3f(v->normal.x, v->normal.y, v->normal.z); glVertex3f(v->pos.x, v->pos.y, v->pos.z);
            v = f->vertices[2];
            glColor3f(v->color.x, v->color.y, v->color.z); glNormal3f(v->normal.x, v->normal.y, v->normal.z); glVertex3f(v->pos.x, v->pos.y, v->pos.z);
        });
        glEnd();
        glShadeModel(GL_FLAT);
    }
}

void Mesh::RenderUVMap(){
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(4.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    vertices.Foreach([](Vertex* v){
        glVertex2f(v->uv.x, v->uv.y);
    });
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    edges.Foreach([](Edge* e){
        glVertex2f(e->v1->uv.x, e->v1->uv.y);
        glVertex2f(e->v2->uv.x, e->v2->uv.y);
    });
    glEnd();
    glDisable(GL_LINE_SMOOTH);
}

void Mesh::WriteToOBJ(HANDLE hFile, bool uv, bool normal){
    size_t index = 0;
    WriteFile(hFile, "# StereoVision 3D Editor\n", 25, NULL, NULL);
    WriteFile(hFile, "# Author: lin-boheng@gitee.com\n", 31, NULL, NULL);
    // 顶点索引
    vertices.Foreach<size_t*>([](Vertex* v, size_t* i){
        v->index = ++*i;
    }, &index);
    // 顶点
    vertices.Foreach<HANDLE>([](Vertex* v, HANDLE hFile){
        char tmp[MAX_PATH + 1];
        DWORD len;
        len = __builtin_snprintf(tmp, MAX_PATH, "v %f %f %f\n", v->pos.x, v->pos.y, v->pos.z);
        WriteFile(hFile, tmp, len, &len, NULL);
    }, hFile);
    // 纹理UV坐标
    if (uv){
        vertices.Foreach<HANDLE>([](Vertex* v, HANDLE hFile){
            char tmp[MAX_PATH + 1];
            DWORD len;
            len = __builtin_snprintf(tmp, MAX_PATH, "vt %f %f\n", v->uv.x, v->uv.y);
            WriteFile(hFile, tmp, len, &len, NULL);
        }, hFile);
    }
    // 顶点法线
    if (normal){
        vertices.Foreach<HANDLE>([](Vertex* v, HANDLE hFile){
            char tmp[MAX_PATH + 1];
            DWORD len;
            len = __builtin_snprintf(tmp, MAX_PATH, "vn %f %f %f\n", v->normal.x, v->normal.y, v->normal.z);
            WriteFile(hFile, tmp, len, &len, NULL);
        }, hFile);
    }
    // 片元
    if (!uv && !normal){
        faces.Foreach<HANDLE>([](Face* f, HANDLE hFile){
            char tmp[MAX_PATH + 1];
            DWORD len;
            int idx1 = f->vertices[0]->index, idx2 = f->vertices[1]->index, idx3 = f->vertices[2]->index;
            len = __builtin_snprintf(tmp, MAX_PATH, "f %d %d %d\n", idx1, idx2, idx3);
            WriteFile(hFile, tmp, len, &len, NULL);
        }, hFile);
    }else if (uv && !normal){
        faces.Foreach<HANDLE>([](Face* f, HANDLE hFile){
            char tmp[MAX_PATH + 1];
            DWORD len;
            int idx1 = f->vertices[0]->index, idx2 = f->vertices[1]->index, idx3 = f->vertices[2]->index;
            len = __builtin_snprintf(tmp, MAX_PATH, "f %d/%d %d/%d %d/%d\n", idx1, idx1, idx2, idx2, idx3, idx3);
            WriteFile(hFile, tmp, len, &len, NULL);
        }, hFile);
    }else if (!uv && normal){
        faces.Foreach<HANDLE>([](Face* f, HANDLE hFile){
            char tmp[MAX_PATH + 1];
            DWORD len;
            int idx1 = f->vertices[0]->index, idx2 = f->vertices[1]->index, idx3 = f->vertices[2]->index;
            len = __builtin_snprintf(tmp, MAX_PATH, "f %d//%d %d//%d %d//%d\n", idx1, idx1, idx2, idx2, idx3, idx3);
            WriteFile(hFile, tmp, len, &len, NULL);
        }, hFile);
    }else{
        faces.Foreach<HANDLE>([](Face* f, HANDLE hFile){
            char tmp[MAX_PATH + 1];
            DWORD len;
            int idx1 = f->vertices[0]->index, idx2 = f->vertices[1]->index, idx3 = f->vertices[2]->index;
            len = __builtin_snprintf(tmp, MAX_PATH, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", idx1, idx1, idx1, idx2, idx2, idx2, idx3, idx3, idx3);
            WriteFile(hFile, tmp, len, &len, NULL);
        }, hFile);
    }
    
}

void Mesh::SetTexture(int resid){
    if (resid == -1){
        if (modeltex){
            delete modeltex;
            modeltex = NULL;
        }
        return;
    }
    if (modeltex){
        delete modeltex;
    }
    modeltex = new GLTexture2D(resid);
}

void Mesh::SetTexture(GLTexture2D* texture){
    if (modeltex)
        delete modeltex;
    modeltex = texture;
}

void Mesh::ResetTexture(){
    if (modeltex){
        delete modeltex;
        modeltex = NULL;
    }
}

bool Mesh::EnableTexture(){
    if (!modeltex)
        return false;
    modeltex->Enable();
    return true;
}

bool Mesh::DisableTexture(){
    GLTexture2D::Disable();
    return true;
}