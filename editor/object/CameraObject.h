#ifndef __EDITOR_OBJECT_CAMERAOBJECT__
#define __EDITOR_OBJECT_CAMERAOBJECT__

#include <define.h>

#include <utils/math3d/Geometry.h>
#include <editor/main/ViewObject.h>

class CameraObject final : public AViewObject {
private:
    Vertex pos;
    Vertex lookAt;

public:
    CameraObject();
    virtual ~CameraObject() override;

    virtual void OnSelect(Point3 ori, Vector3 dir) override;
    virtual void OnSelect(const SelectInfo* info) override;

    virtual void OnRender() override;
};

#endif