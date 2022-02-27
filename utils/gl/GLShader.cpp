#include <utils/gl/GLShader.h>

#include <lib/opengl/gl/gl.h>
#include <utils/os/GLFunc.h>

GLShader::GLShader(uenum stage){
    shader = glCreateShader(stage);
}

GLShader::~GLShader(){
    glDeleteShader(shader);
}