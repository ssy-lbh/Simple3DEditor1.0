#include <utils/gl/GLProgram.h>

#include <lib/opengl/gl/gl.h>

#include <utils/os/GLFunc.h>
#include <utils/os/Log.h>
#include <utils/gl/GLShader.h>

GLProgram::GLProgram(){
    prog = glCreateProgram();
}

GLProgram::~GLProgram(){
    glDeleteProgram(prog);
}

void GLProgram::AddShader(GLShader* shader){
    glAttachShader(prog, shader->GetShader());
}

void GLProgram::RemoveShader(GLShader* shader){
    glDetachShader(prog, shader->GetShader());
}

bool GLProgram::Link(){
    GLint state;

    glGetProgramiv(prog, GL_LINK_STATUS, &state);
    return state == GL_FALSE;
}

void GLProgram::PrintLog(){
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

uint GLProgram::GetProgram(){
    return prog;
}