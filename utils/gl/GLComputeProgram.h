#ifndef __UTILS_GL_GLCOMPUTEPROGRAM__
#define __UTILS_GL_GLCOMPUTEPROGRAM__

#include <define.h>

class GLComputeProgram final : public Object {
private:
    uint prog;
    uint shader;

    int progLog = -1;
    int shaderLog = -1;

public:
    GLComputeProgram(int resid);
    GLComputeProgram(const char* source);
    ~GLComputeProgram();

    uint GetProgram();
    bool CheckProgramError();
    bool CheckShaderError();
    void PrintProgramLog();
    void PrintShaderLog();
    void Dispatch(int x, int y, int z);
    uint GetLoc(const char* name);
    void BindTexture(uint unit, uint texture, uenum access, uenum format);
};

#endif