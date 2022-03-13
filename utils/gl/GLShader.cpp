#include <utils/gl/GLShader.h>

#include <lib/glew/gl/glew.h>

#include <utils/String.h>
#include <utils/DataBuffer.h>
#include <utils/os/Log.h>
#include <utils/os/Resource.h>

GLShader::GLShader(uenum stage){
    shader = glCreateShader(stage);
}

GLShader::GLShader(int resid, uenum stage){
    shader = glCreateShader(stage);
    Source(Resource::GetShader(resid));
}

GLShader::~GLShader(){
    glDeleteShader(shader);
}

void GLShader::Source(String src){
    const char* source = src.GetString();
    glShaderSource(shader, 1, &source, NULL);
}

void GLShader::Source(DataBuffer src){
    const char* source = (const char*)src.Buffer();
    glShaderSource(shader, 1, &source, NULL);
}

bool GLShader::Compile(){
    GLint state;

    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &state);
    return state == GL_FALSE;
}

void GLShader::PrintLog(){
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

uint GLShader::GetShader(){
    return shader;
}

uenum GLShader::GetStage(){
    int stage;
    glGetShaderiv(shader, GL_SHADER_TYPE, &stage);
    return (uenum)stage;
}