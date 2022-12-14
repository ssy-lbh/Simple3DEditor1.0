#ifndef __UTIL_GL_GLFRAMEBUFFER__
#define __UTIL_GL_GLFRAMEBUFFER__

#include <define.h>

#include <util/math3d/LinearAlgebra.h>

namespace simple3deditor {

class GLFrameBuffer final : public Object {
private:
    uint frame = 0;
    uint buffer;
    Rect rect = Rect::zero;

public:
    GLFrameBuffer();
    GLFrameBuffer(GLRenderTexture2D* texture);
    ~GLFrameBuffer();

    void BindTexture(GLRenderTexture2D* texture);
    void BindTexture(GLRenderTexture2D* texture, uenum attachment);
    void SetSize(Rect rect);
    void SetSize(GLRenderTexture2D* texture);
    void Enable();
    void Disable();
    void EnableRenderBuffer();
    void DisableRenderBuffer();
};

}

#endif