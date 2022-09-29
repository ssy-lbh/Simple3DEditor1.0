#include <util/gl/GLComputeProgram.h>

#include <glew/gl/glew.h>

#include <res.h>
#include <util/DataBuffer.h>
#include <util/os/Resource.h>
#include <util/os/Log.h>
#include <util/gl/GLTexture2D.h>
#include <util/gl/GLRenderTexture2D.h>

namespace simple3deditor {

GLComputeProgram::GLComputeProgram(int resid){
    int resSize;
    const char* srcData;

    DataBuffer data = Resource::GetShader(resid);
    srcData = (const char*)data.ReadOnlyBuffer();
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
    if (progLog == -1)
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &progLog);
    if (progLog > 0){
        char* log = new char[progLog];
        glGetProgramInfoLog(prog, progLog, &progLog, log);
        DebugError("%s", log);
        delete[] log;
    }
}

void GLComputeProgram::PrintShaderLog(){
    if (shaderLog == -1)
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderLog);
    if (shaderLog > 0){
        char* log = new char[shaderLog];
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
    // Ê¡ÂÔÁËglActiveTexture(GL_TEXTURE0);
    glBindImageTexture(unit, texture, 0, GL_FALSE, 0, access, format);
    glDisable(GL_TEXTURE_2D);
}

void GLComputeProgram::BindTexture(uint unit, GLTexture2D* texture, uenum access, uenum format){
    BindTexture(unit, texture->GetTexture(), access, format);
}

void GLComputeProgram::BindTexture(uint unit, GLRenderTexture2D* texture, uenum access, uenum format){
    BindTexture(unit, texture->GetTexture(), access, format);
}

}