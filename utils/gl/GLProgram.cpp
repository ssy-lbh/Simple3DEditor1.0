#include <utils/gl/GLProgram.h>

#include <lib/opengl/gl/gl.h>
#include <utils/os/GLFunc.h>

GLProgram::GLProgram(){
    prog = glCreateProgram();
}

GLProgram::~GLProgram(){
    glDeleteProgram(prog);
}