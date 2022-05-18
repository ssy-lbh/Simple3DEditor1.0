#ifndef __UTIL_GL_GLVERTEXBUFFER__
#define __UTIL_GL_GLVERTEXBUFFER__

#include <define.h>

namespace simple3deditor {

class GLVertexBuffer final : public Object {
private:
    uint buffer;

public:
    GLVertexBuffer();
    ~GLVertexBuffer();

    uint GetBuffer();
};

}

#endif