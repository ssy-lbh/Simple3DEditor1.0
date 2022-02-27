#ifndef __UTILS_GL_GLFRAMEBUFFER__
#define __UTILS_GL_GLFRAMEBUFFER__

#include <define.h>

class GLFrameBuffer final : public Object {
private:
    uint frame = 0;
    bool hasScale = false;
    int x = 0, y = 0;

public:
    GLFrameBuffer();
    GLFrameBuffer(uint texture);
    ~GLFrameBuffer();

    void BindTexture(uint texture);
    void BindTexture(uint texture, int x, int y);
    void BindTexture(uint texture, uenum attachment);
    void BindTexture(uint texture, uenum attachment, int x, int y);
    void Enable();
    void Disable();
};

#endif