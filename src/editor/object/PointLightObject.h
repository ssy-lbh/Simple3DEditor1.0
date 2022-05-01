#ifndef __EDITOR_OBJECT_POINTLIGHTOBJECT__
#define __EDITOR_OBJECT_POINTLIGHTOBJECT__

#include <define.h>

#include <util/math3d/Geometry.h>
#include <editor/main/ViewObject.h>

class PointLightObject final : public AViewObject {
private:
    Vertex v;
    uenum light;

public:
    PointLightObject();
    virtual ~PointLightObject() override;

    virtual void OnSelect(Point3 ori, Vector3 dir) override;
    virtual void OnSelect(const SelectInfo* info) override;

    virtual void OnRender() override;

    virtual void OnTimer(int id) override;

    void UpdateLight();
};

#endif