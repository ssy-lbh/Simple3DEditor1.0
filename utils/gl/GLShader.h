#ifndef __UTILS_GL_GLSHADER__
#define __UTILS_GL_GLSHADER__

#include <define.h>

class GLShader final : public Object {
private:
    uint shader;
    int shaderLog = -1;

public:
    GLShader(uenum stage);
    GLShader(int resid, uenum stage);
    ~GLShader();

    void Source(String src);
    void Source(DataBuffer src);
    bool Compile();
    void PrintLog();
    uint GetShader();
    uenum GetStage();
};

#endif