#ifndef __UTILS_GL_GLPROGRAM__
#define __UTILS_GL_GLPROGRAM__

#include <define.h>

class GLProgram final : public Object {
private:
    uint prog;

public:
    GLProgram();
    ~GLProgram();
};

#endif