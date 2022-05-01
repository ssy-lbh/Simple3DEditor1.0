#ifndef __EDITOR_OBJECT_GUIMESHOBJECT__
#define __EDITOR_OBJECT_GUIMESHOBJECT__

#include <define.h>

#include <util/gl/GLRenderTexture2D.h>
#include <util/gl/GLFrameBuffer.h>
#include <editor/object/GUIManagerObject.h>

class GUIMeshObject final : public GUIManagerObject {
protected:
    Mesh* mesh;

    GLRenderTexture2D* tex;
    GLFrameBuffer* frame;

    virtual bool UpdateCursor3D(Point3 ori, Vector3 dir) override;
    void UpdateCursor(Vector2 pos);

public:
    GUIMeshObject();
    virtual ~GUIMeshObject() override;

    virtual void OnSelect(Point3 ori, Vector3 dir) override;
    virtual void OnSelect(const SelectInfo* info) override;
    virtual void OnSelectUV(Vector2 uv, float err = 0.01f) override;
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2) override;

    virtual void OnChainRender() override;
    virtual void OnRender() override;
    virtual void OnRenderUVMap() override;

    Mesh* GetMesh();
};

#endif