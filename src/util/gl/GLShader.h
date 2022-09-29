#ifndef __UTIL_GL_GLSHADER__
#define __UTIL_GL_GLSHADER__

#include <define.h>

namespace simple3deditor {

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
    // 发生错误返回true
    bool Compile();
    // OpenGL 4.6
    bool CompileSPIRV(DataBuffer src, const char* entry);
    bool CompileSPIRV(DataBuffer src, String entry);
    void PrintLog();
    uint GetShader();
    uenum GetStage();
};

}

#endif