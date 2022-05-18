#include <editor/object/MeshObject.h>

// GLUT依赖于C Runtime
#include <crtdefs.h>
#include <lib/glut/glut.h>

#include <main.h>
#include <util/math3d/Mesh.h>
#include <util/gl/GLSimplified.h>
#include <util/os/Log.h>

namespace simple3deditor {

MeshObject::MeshObject() : AViewObject(L"Mesh", ViewObjectType::OBJECT_MESH) {
    mesh = new Mesh(this);
}

MeshObject::MeshObject(const wchar_t* name, ViewObjectType type) : AViewObject(name, type) {
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
    case SelectionType::SELECT_VERTICES:
        mesh->OnSelectVertex(ori, dir, Main::data->selPoints);
        break;
    case SelectionType::SELECT_EDGES:
        mesh->OnSelectEdge(ori, dir, Main::data->selEdges);
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
    mesh->OnSelectUV(uv, Main::data->selPoints, err);
}

void MeshObject::OnSelectUV(Vector2 uv1, Vector2 uv2){
    mesh->FindUVRect(uv1, uv2, Main::data->selPoints);
}

void MeshObject::OnRender(){
    AViewObject::OnRender();
    mesh->Render();
}

void MeshObject::OnRenderUVMap(){
    AViewObject::OnRenderUVMap();
    mesh->RenderUVMap();
}

Mesh* MeshObject::GetMesh(){
    return mesh;
}

}