#ifndef __UTILS_GL_GLVERTEXARRAY__
#define __UTILS_GL_GLVERTEXARRAY__

#include <define.h>

class GLVertexArray final : public Object {
private:
    uint array;

public:
    GLVertexArray();
    ~GLVertexArray();

    void BindBuffer(GLVertexBuffer* buffer);
    void SetFormat(uint idx, uint size, uenum type, uint offset);
    uint GetArray();
};

#endif