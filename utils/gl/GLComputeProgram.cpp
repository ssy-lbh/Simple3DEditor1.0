#include <utils/gl/GLComputeProgram.h>

#include <lib/opengl/gl/gl.h>

#include <res.h>
#include <utils/DataBuffer.h>
#include <utils/os/Resource.h>
#include <utils/os/Log.h>
#include <utils/os/GLFunc.h>
#include <utils/gl/GLTexture2D.h>
#include <utils/gl/GLRenderTexture2D.h>

GLComputeProgram::GLComputeProgram(int resid){
    int resSize;
    char* srcData;

    DataBuffer data = Resource::GetShader(resid);
    srcData = (char*)data.Buffer();
    resSize = data.Size();

    prog = glCreateProgram();
    shader = glCreateShader(GL_COMPUTE_SHADER);

    glShaderSource(shader, 1, &srcData, &resSize);
}

GLComputeProgram::GLComputeProgram(const char* source){
    prog = glCreateProgram();
    shader = glCreateShader(GL_COMPUTE_SHADER);

    glShaderSource(shader, 1, &source, NULL);
}

GLComputeProgram::~GLComputeProgram(){
    glDeleteProgram(prog);
    glDeleteShader(shader);
}

GLuint GLComputeProgram::GetProgram(){
    return prog;
}

bool GLComputeProgram::LinkProgram(){
    GLint status;

    glAttachShader(prog, shader);
    glLinkProgram(prog);
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    return status == GL_FALSE;
}

bool GLComputeProgram::CompileShader(){
    GLint status;

    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    return status == GL_FALSE;
}

void GLComputeProgram::PrintProgramLog(){
    char* log;

    if (progLog == -1)
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &progLog);
    if (progLog > 0){
        log = new char[progLog];
        glGetProgramInfoLog(prog, progLog, &progLog, log);
        DebugError("%s", log);
        delete[] log;
    }
}

void GLComputeProgram::PrintShaderLog(){
    char* log;

    if (shaderLog == -1)
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderLog);
    if (shaderLog > 0){
        log = new char[shaderLog];
        glGetShaderInfoLog(shader, shaderLog, &shaderLog, log);
        DebugError("%s", log);
        delete[] log;
    }
}

void GLComputeProgram::Dispatch(int x, int y, int z){
    glUseProgram(prog);
    glDispatchCompute(x, y, z);
}

GLuint GLComputeProgram::GetLoc(const char* name){
    return glGetUniformLocation(prog, name);
}

void GLComputeProgram::BindTexture(uint unit, uint texture, uenum access, uenum format){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 省略了glActiveTexture(GL_TEXTURE0);
    glBindImageTexture(unit, texture, 0, GL_FALSE, 0, access, format);
    glDisable(GL_TEXTURE_2D);
}

void GLComputeProgram::BindTexture(uint unit, GLTexture2D* texture, uenum access, uenum format){
    BindTexture(unit, texture->GetTexture(), access, format);
}

void GLComputeProgram::BindTexture(uint unit, GLRenderTexture2D* texture, uenum access, uenum format){
    BindTexture(unit, texture->GetTexture(), access, format);
}