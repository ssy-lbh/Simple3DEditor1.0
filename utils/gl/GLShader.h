#ifndef __UTILS_GL_GLSHADER__
#define __UTILS_GL_GLSHADER__

#include <define.h>

class GLShader final : public Object {
private:
    uint shader;

public:
    GLShader(uenum stage);
    ~GLShader();
};

#endif