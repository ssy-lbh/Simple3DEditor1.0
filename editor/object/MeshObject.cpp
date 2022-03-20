#include <editor/object/MeshObject.h>

#include <main.h>
#include <utils/math3d/Mesh.h>
#include <utils/gl/GLSimplified.h>
#include <utils/os/Log.h>

MeshObject::MeshObject() : AViewObject(L"Mesh", ViewObjectType::OBJECT_MESH) {
    mesh = new Mesh(this);
}

MeshObject::~MeshObject(){
    // 子类析构器在返回前会自动调用父类析构器
    // 因此不要再这里调用AViewObject::~AViewObject，不然会导致多次delete使程序崩溃
    if (mesh) delete mesh;
}

void MeshObject::OnSelect(Point3 ori, Vector3 dir){
    ori = transform.chainInvMat * ori;
    dir = transform.chainInvMat * dir;

    switch (Main::data->selType){
    case SelectionType::SELECT_VERTICES:{
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
    case SelectionType::SELECT_EDGES:{
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
    case SelectionType::SELECT_VERTICES:
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

void MeshObject::OnRender(){
    AViewObject::OnRender();

    mesh->Render();
}

void MeshObject::OnRenderUVMap(){
    AViewObject::OnRenderUVMap();
    mesh->RenderUVMap();
}