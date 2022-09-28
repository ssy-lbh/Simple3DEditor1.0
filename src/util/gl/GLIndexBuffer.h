#ifndef __UTIL_GL_GLINDEXBUFFER__
#define __UTIL_GL_GLINDEXBUFFER__

#include <define.h>

namespace simple3deditor {

class GLIndexBuffer final : public Object {
private:
    uint buffer;

public:
    GLIndexBuffer();
    ~GLIndexBuffer();
};

}

#endif