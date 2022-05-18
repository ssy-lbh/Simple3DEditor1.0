#ifndef __EDITOR_OBJECT_MESHOBJECT__
#define __EDITOR_OBJECT_MESHOBJECT__

#include <define.h>

#include <editor/main/ViewObject.h>

namespace simple3deditor {

class MeshObject : public AViewObject {
protected:
    Mesh* mesh;

    MeshObject(const wchar_t* name, ViewObjectType type);

public:
    MeshObject();
    virtual ~MeshObject() override;

    virtual void OnSelect(Point3 ori, Vector3 dir) override;
    virtual void OnSelect(const SelectInfo* info) override;
    virtual void OnSelectUV(Vector2 uv, float err = 0.01f) override;
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2) override;

    virtual void OnRender() override;
    virtual void OnRenderUVMap() override;

    Mesh* GetMesh();
};

}

#endif