#ifndef __UTILS_GL_GLPROGRAM__
#define __UTILS_GL_GLPROGRAM__

#include <define.h>

class GLProgram final : public Object {
private:
    uint prog;
    int progLog = -1;

public:
    GLProgram();
    ~GLProgram();

    void AddShader(GLShader* shader);
    void RemoveShader(GLShader* shader);
    bool Link();
    void PrintLog();
    uint GetProgram();
};

#endif