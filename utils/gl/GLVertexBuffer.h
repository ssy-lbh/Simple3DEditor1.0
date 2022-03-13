#ifndef __UTILS_GL_GLVERTEXBUFFER__
#define __UTILS_GL_GLVERTEXBUFFER__

#include <define.h>

class GLVertexBuffer final : public Object {
private:
    uint buffer;

public:
    GLVertexBuffer();
    ~GLVertexBuffer();

    uint GetBuffer();
};

#endif