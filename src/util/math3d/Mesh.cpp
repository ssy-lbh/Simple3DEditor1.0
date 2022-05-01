#include <util/math3d/Mesh.h>

#include <lib/opengl/gl/gl.h>

#include <main.h>
#include <res.h>
#include <io/File.h>
#include <util/os/Log.h>
#include <util/gl/GLUtils.h>
#include <util/gl/GLTexture2D.h>
#include <util/math3d/Math.h>
#include <util/math3d/LinearAlgebra.h>
#include <util/math3d/Geometry.h>
#include <editor/main/ViewObject.h>

Mesh::Mesh(){}

Mesh::Mesh(AViewObject* object) : object(object) {}

Mesh::~Mesh(){
    if (modeltex) delete modeltex;
    Free(vertices);
    Free(edges);
    Free(faces);
}

Vertex* Mesh::Find(Vector3 ori, Vector3 dir){
    float selectDis = -1.0f;
    Vertex* res;

    vertices.Foreach([&](Vertex* p){
        if (Vector3::Cosine(dir, p->pos - ori) > 0.9997f){
            if (selectDis < 0.0f || (p->pos - ori).SqrMagnitude() < selectDis * selectDis){
                selectDis = (p->pos - ori).Magnitude();
                res = p;
            }
        }
    });
    return selectDis < 0.0f ? NULL : res;
}

size_t Mesh::FindScreenRect(const SelectInfo* info, Matrix4x4 mat, List<Vertex*>& result){
    size_t cnt = 0;

    vertices.Foreach([&](Vertex* p){
        if (info->Inside(mat * Point3(p->pos))){
            if (result.HasValue(p))
                return;
            cnt++;
            result.Add(p);
        }
    });
    return cnt;
}

Edge* Mesh::FindEdge(Vector3 ori, Vector3 dir){
    Edge* res = NULL;

    edges.Foreach([&](Edge* e){
        if (e->Hit(ori, dir))
            res = e;
    });
    return res;
}

Vertex* Mesh::FindUV(Vector2 uv, float err){
    err *= err;
    Vertex* res = NULL;

    vertices.Foreach([=, &res](Vertex* p){
        if (res)
            return;
        if ((p->uv - uv).SqrMagnitude() <= err){
            res = p;
        }
    });
    return res;
}

size_t Mesh::FindUVRect(Vector2 uv1, Vector2 uv2, List<Vertex*>& result){
    size_t cnt = 0;
    float x1 = uv1.x, x2 = uv2.x, y1 = uv1.y, y2 = uv2.y;
    List<Vertex*>* list;

    Sort(x1, x2); Sort(y1, y2);
    
    vertices.Foreach([&](Vertex* p){
        if (p->uv.x >= x1 && p->uv.x <= x2 && p->uv.y >= y1 && p->uv.y <= y2){
            cnt++;
            result.Add(p);
        }
    });
    return cnt;
}

Face* Mesh::Intersect(Point3 ori, Vector3 dir, Vector3* bary){
    float dis, mindis;
    Vector3 tmp;
    Face* res = NULL;
    size_t size = faces.Size();

    for (size_t i = 0; i < size; i++){
        if (faces[i]->Intersect(ori, dir, &tmp, &dis)){
            if (!res){
                mindis = dis;
                res = faces[i];
                if (bary) *bary = tmp;
            }else if (dis < mindis){
                mindis = dis;
                res = faces[i];
                if (bary) *bary = tmp;
            }
        }
    }

    return res;
}

bool Mesh::IntersectUV(Point3 ori, Vector3 dir, Vector2* uv){
    Vector3 bary;
    Face* f = Intersect(ori, dir, &bary);

    if (f){
        *uv = f->vertices[0]->uv * bary.x
                + f->vertices[1]->uv * bary.y
                + f->vertices[2]->uv * bary.z;
        return true;
    }

    return false;
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
    bool has = false;
    Edge* res;

    v1->edges.Foreach([=, &has, &res](Edge* e){
        if (e->v1 == v1 && e->v2 == v2){
            has = true;
            res = e;
        }else if (e->v1 == v2 && e->v2 == v1){
            has = true;
            res = e;
        }
    });
    if (has)
        return res;
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
    v->faces.Foreach([=](Face* f){
        //DebugLog("Face::DeleteSelfReferenceExcept %p", f);
        f->DeleteSelfReferenceExcept(v);
        this->faces.Remove(f);
        delete f;
    });
    v->edges.Foreach([=](Edge* e){
        //DebugLog("Edge::DeleteSelfReferenceExcept %p", e);
        e->DeleteSelfReferenceExcept(v);
        this->edges.Remove(e);
        delete e;
    });
    vertices.Remove(v);
    delete v;
}

void Mesh::DeleteEdge(Edge* e){
    e->faces.Foreach([=](Face* f){
        f->DeleteSelfReferenceExcept(e);
        this->faces.Remove(f);
        delete f;
    });
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

void Mesh::OnSelectVertex(Point3 ori, Vector3 dir, List<Vertex*>& list){
    Vertex* v = Find(ori, dir);
    if (!v){
        list.Clear();
        DebugLog("No Point Selected");
    }else{
        if (list.HasValue(v))
            return;
        list.Add(v);
        DebugLog("Select Point %f %f %f", v->pos.x, v->pos.y, v->pos.z);
    }
}

void Mesh::OnSelectEdge(Point3 ori, Vector3 dir, List<Edge*>& list){
    Edge* e = FindEdge(ori, dir);
    if (!e){
        list.Clear();
        DebugLog("No Edge Selected");
    }else{
        if (list.HasValue(e))
            return;
        list.Add(e);
        DebugLog("Select Edge (%f,%f,%f) (%f,%f,%f)", e->v1->pos.x, e->v1->pos.y, e->v1->pos.z, e->v2->pos.x, e->v2->pos.y, e->v2->pos.z);
    }
}

void Mesh::OnSelectUV(Vector2 uv, List<Vertex*>& list, float err){
    Vertex* v = FindUV(uv, err);
    if (!v){
        list.Clear();
        DebugLog("No Point Selected");
    }else{
        if (list.HasValue(v))
            return;
        list.Add(v);
        DebugLog("Select Point %f %f", v->uv.x, v->uv.y);
    }
}

void Mesh::Render(){
    Render(&LocalData::GetLocalInst()->renderOptions);
}

void Mesh::Render(const RenderOptions* options){
    glDisable(GL_LIGHTING);

    if (options->vertex){
        glEnable(GL_POINT_SMOOTH);
        glPointSize(4.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POINTS);
        vertices.Foreach([](Vertex* v){
            glVertex3f(v->pos.x, v->pos.y, v->pos.z);
        });
        glEnd();
        glDisable(GL_POINT_SMOOTH);
    }
    
    if (options->edge){
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
    }

    if (options->face){
        if (options->light)
            glEnable(GL_LIGHTING);// 开启光照系统
        glShadeModel(GL_SMOOTH);
        if (EnableTexture()){
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
            DisableTexture();
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
        }
        glShadeModel(GL_FLAT);
    }
}

void Mesh::RenderWithUV(){
    RenderWithUV(&LocalData::GetLocalInst()->renderOptions);
}

void Mesh::RenderWithUV(const RenderOptions* options){
    glDisable(GL_LIGHTING);

    if (options->vertex){
        glEnable(GL_POINT_SMOOTH);
        glPointSize(4.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POINTS);
        vertices.Foreach([](Vertex* v){
            glVertex3f(v->pos.x, v->pos.y, v->pos.z);
        });
        glEnd();
        glDisable(GL_POINT_SMOOTH);
    }
    
    if (options->edge){
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
    }

    if (options->face){
        if (options->light)
            glEnable(GL_LIGHTING);// 开启光照系统
        glShadeModel(GL_SMOOTH);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        faces.Foreach([](Face* f){
            Vertex* v;
            // V坐标已经反转
            v = f->vertices[0];
            glTexCoord2f(v->uv.x, v->uv.y); glColor3f(v->color.x, v->color.y, v->color.z); glNormal3f(v->normal.x, v->normal.y, v->normal.z); glVertex3f(v->pos.x, v->pos.y, v->pos.z);
            v = f->vertices[1];
            glTexCoord2f(v->uv.x, v->uv.y); glColor3f(v->color.x, v->color.y, v->color.z); glNormal3f(v->normal.x, v->normal.y, v->normal.z); glVertex3f(v->pos.x, v->pos.y, v->pos.z);
            v = f->vertices[2];
            glTexCoord2f(v->uv.x, v->uv.y); glColor3f(v->color.x, v->color.y, v->color.z); glNormal3f(v->normal.x, v->normal.y, v->normal.z); glVertex3f(v->pos.x, v->pos.y, v->pos.z);
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

void Mesh::WriteToOBJ(File file, bool uv, bool normal){
    size_t index = 0;
    file.Write("# StereoVision 3D Editor\n", 25);
    file.Write("# Author: lin-boheng@gitee.com\n", 31);
    // 顶点索引
    vertices.Foreach([&index](Vertex* v){
        v->index = ++index;
    });
    // 顶点
    vertices.Foreach([&file](Vertex* v){
        char tmp[MAX_PATH + 1];
        DWORD len;
        len = __builtin_snprintf(tmp, MAX_PATH, "v %f %f %f\n", v->pos.x, v->pos.y, v->pos.z);
        file.Write(tmp, len);
    });
    // 纹理UV坐标
    if (uv){
        vertices.Foreach([&file](Vertex* v){
            char tmp[MAX_PATH + 1];
            DWORD len;
            len = __builtin_snprintf(tmp, MAX_PATH, "vt %f %f\n", v->uv.x, v->uv.y);
            file.Write(tmp, len);
        });
    }
    // 顶点法线
    if (normal){
        vertices.Foreach([&file](Vertex* v){
            char tmp[MAX_PATH + 1];
            DWORD len;
            len = __builtin_snprintf(tmp, MAX_PATH, "vn %f %f %f\n", v->normal.x, v->normal.y, v->normal.z);
            file.Write(tmp, len);
        });
    }
    // 片元
    if (!uv && !normal){
        faces.Foreach([&file](Face* f){
            char tmp[MAX_PATH + 1];
            DWORD len;
            int idx1 = f->vertices[0]->index, idx2 = f->vertices[1]->index, idx3 = f->vertices[2]->index;
            len = __builtin_snprintf(tmp, MAX_PATH, "f %d %d %d\n", idx1, idx2, idx3);
            file.Write(tmp, len);
        });
    }else if (uv && !normal){
        faces.Foreach([&file](Face* f){
            char tmp[MAX_PATH + 1];
            DWORD len;
            int idx1 = f->vertices[0]->index, idx2 = f->vertices[1]->index, idx3 = f->vertices[2]->index;
            len = __builtin_snprintf(tmp, MAX_PATH, "f %d/%d %d/%d %d/%d\n", idx1, idx1, idx2, idx2, idx3, idx3);
            file.Write(tmp, len);
        });
    }else if (!uv && normal){
        faces.Foreach([&file](Face* f){
            char tmp[MAX_PATH + 1];
            DWORD len;
            int idx1 = f->vertices[0]->index, idx2 = f->vertices[1]->index, idx3 = f->vertices[2]->index;
            len = __builtin_snprintf(tmp, MAX_PATH, "f %d//%d %d//%d %d//%d\n", idx1, idx1, idx2, idx2, idx3, idx3);
            file.Write(tmp, len);
        });
    }else{
        faces.Foreach([&file](Face* f){
            char tmp[MAX_PATH + 1];
            DWORD len;
            int idx1 = f->vertices[0]->index, idx2 = f->vertices[1]->index, idx3 = f->vertices[2]->index;
            len = __builtin_snprintf(tmp, MAX_PATH, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", idx1, idx1, idx1, idx2, idx2, idx2, idx3, idx3, idx3);
            file.Write(tmp, len);
        });
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
    if (!modeltex->Enable()){
        delete modeltex;
        modeltex = NULL;
        return false;
    }
    return true;
}

bool Mesh::DisableTexture(){
    GLTexture2D::Disable();
    return true;
}