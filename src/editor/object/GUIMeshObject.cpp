#include <editor/object/GUIMeshObject.h>

#include <lib/glew/gl/glew.h>

#include <exception>

#include <main.h>
#include <util/gl/GLSimplified.h>
#include <util/gl/GLUtils.h>
#include <util/gl/GLTexture2D.h>
#include <util/gl/GLRenderTexture2D.h>
#include <util/gl/GLFrameBuffer.h>
#include <util/os/Log.h>
#include <util/os/Font.h>
#include <util/math3d/Mesh.h>
#include <editor/main/ViewManager.h>
#include <editor/object/GUIObject.h>

namespace simple3deditor {

GUIMeshObject::GUIMeshObject() : GUIManagerObject(L"GUIMesh", ViewObjectType::OBJECT_GUI_MESH) {
    mesh = new Mesh(this);
    tex = new GLRenderTexture2D(500, 500, GL_RGBA32F);
    frame = new GLFrameBuffer(tex);
    frame->EnableRenderBuffer();
}

GUIMeshObject::~GUIMeshObject(){
    if (mesh) delete mesh;
    if (frame) delete frame;
    if (tex) delete tex;
}

bool GUIMeshObject::UpdateCursor3D(Point3 ori, Vector3 dir){
    Point3 camPos = transform.chainInvMat * ori;
    Vector3 cursorDir = transform.chainInvMat * dir;
    // 高性能消耗，暂时不会优化
    bool res = mesh->IntersectUV(camPos, cursorDir, &cursorPos);
    if (res){
        cursorPos = cursorPos * 2.0f - Vector2::one;
    }
    return res;
}

void GUIMeshObject::UpdateCursor(Vector2 pos){
    UpdateCursor(pos);
}

void GUIMeshObject::OnSelect(Point3 ori, Vector3 dir){
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

void GUIMeshObject::OnSelect(const SelectInfo* info){
    switch (Main::data->selType){
    case SelectionType::SELECT_VERTICES:
        mesh->FindScreenRect(info, transform.chainMat, Main::data->selPoints);
        break;
    }
}

void GUIMeshObject::OnSelectUV(Vector2 uv, float err){
    mesh->OnSelectUV(uv, Main::data->selPoints, err);
}

void GUIMeshObject::OnSelectUV(Vector2 uv1, Vector2 uv2){
    mesh->FindUVRect(uv1, uv2, Main::data->selPoints);
}

void GUIMeshObject::OnChainRender(){
    size_t len = children.Size();

    if (parent){
        transform.chainMat = parent->transform.chainMat * transform.GetMatrix();
        transform.chainInvMat = transform.GetInvMatrix() * parent->transform.chainInvMat;
    }else{
        transform.chainMat = transform.GetMatrix();
        transform.chainInvMat = transform.GetInvMatrix();
    }

    transform.PushMatrix();

    frame->Enable();

    glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
    glClearDepth(1.0);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    for (size_t i = 0; i < len; i++)
        children[i]->OnChainRender();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    frame->Disable();

    OnRender();

    transform.PopMatrix();
}

void GUIMeshObject::OnRender(){
    AViewObject::OnRender();

    tex->Enable();
    mesh->RenderWithUV();
    GLRenderTexture2D::Disable();
}

void GUIMeshObject::OnRenderUVMap(){
    AViewObject::OnRenderUVMap();
    mesh->RenderUVMap();
}

Mesh* GUIMeshObject::GetMesh(){
    return mesh;
}

}