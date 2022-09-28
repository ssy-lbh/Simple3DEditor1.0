#ifndef __EDITOR_OBJECT_SQUAREBEZIEROBJECT__
#define __EDITOR_OBJECT_SQUAREBEZIEROBJECT__

#include <define.h>

#include <util/math3d/Geometry.h>
#include <editor/main/ViewObject.h>

namespace simple3deditor {

class SquareBezierObject final : public AViewObject {
private:
    Vertex v[3];

public:
    OBJECT_INFO_DEF();

    SquareBezierObject();
    virtual ~SquareBezierObject() override;

    virtual void OnSelect(Point3 ori, Vector3 dir) override;
    virtual void OnSelect(const SelectInfo* info) override;
    virtual void OnSelectUV(Vector2 uv, float err = 0.01f) override;
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2) override;

    virtual void OnRender() override;
    virtual void OnRenderUVMap() override;
};

}

#endif