#ifndef __UTILS_GL_GLFRAMEBUFFER__
#define __UTILS_GL_GLFRAMEBUFFER__

#include <define.h>

#include <utils/gl/GLUtils.h>

class GLFrameBuffer final : public Object {
private:
    uint frame = 0;
    GLRect rect = GLRect::zero;

public:
    GLFrameBuffer();
    GLFrameBuffer(GLRenderTexture2D* texture);
    ~GLFrameBuffer();

    void BindTexture(GLRenderTexture2D* texture);
    void BindTexture(GLRenderTexture2D* texture, uenum attachment);
    void SetSize(GLRect rect);
    void SetSize(GLRenderTexture2D* texture);
    void Enable();
    void Disable();
};

#endif