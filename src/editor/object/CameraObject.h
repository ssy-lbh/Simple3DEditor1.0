#ifndef __EDITOR_OBJECT_CAMERAOBJECT__
#define __EDITOR_OBJECT_CAMERAOBJECT__

#include <define.h>

#include <util/math3d/Geometry.h>
#include <editor/main/ViewObject.h>

namespace simple3deditor {

class CameraObject final : public AViewObject {
private:
    Vertex pos;
    Vertex lookAt;

public:
    OBJECT_INFO_DEF();

    CameraObject();
    virtual ~CameraObject() override;

    virtual void OnSelect(Point3 ori, Vector3 dir) override;
    virtual void OnSelect(const SelectInfo* info) override;

    virtual void OnRender() override;
};

}

#endif