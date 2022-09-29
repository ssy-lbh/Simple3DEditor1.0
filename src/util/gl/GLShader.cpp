#include <util/gl/GLShader.h>

#include <glew/gl/glew.h>

#include <util/String.h>
#include <util/DataBuffer.h>
#include <util/os/Log.h>
#include <util/os/Resource.h>

namespace simple3deditor {

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

bool GLShader::CompileSPIRV(DataBuffer src, const char* entry){
    GLint state;

    glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, src.Buffer(), src.Size());
    glSpecializeShader(shader, entry, 0, NULL, NULL);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &state);
    return state == GL_FALSE;
}

bool GLShader::CompileSPIRV(DataBuffer src, String entry){
    return CompileSPIRV(PackDataBuffer(src), entry.GetString());
}

void GLShader::PrintLog(){
    if (shaderLog == -1)
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderLog);
    if (shaderLog > 0){
        char* log = new char[shaderLog];
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

}