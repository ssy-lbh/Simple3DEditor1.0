#ifndef __EDITOR_OBJECT_CUBICBEZIEROBJECT__
#define __EDITOR_OBJECT_CUBICBEZIEROBJECT__

#include <define.h>

#include <util/math3d/Geometry.h>
#include <editor/main/ViewObject.h>

namespace simple3deditor {

class CubicBezierObject final : public AViewObject {
private:
    Vertex v[4];

public:
    CubicBezierObject();
    virtual ~CubicBezierObject() override;

    virtual void OnSelect(Point3 ori, Vector3 dir) override;
    virtual void OnSelect(const SelectInfo* info) override;
    virtual void OnSelectUV(Vector2 uv, float err = 0.01f) override;
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2) override;

    virtual void OnRender() override;
    virtual void OnRenderUVMap() override;
};

}

#endif