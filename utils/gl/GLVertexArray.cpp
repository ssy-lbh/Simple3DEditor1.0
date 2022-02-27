#include <utils/gl/GLVertexArray.h>

#include <lib/opengl/gl/gl.h>
#include <utils/os/GLFunc.h>

GLVertexArray::GLVertexArray(){
    glGenVertexArrays(1, &array);
}

GLVertexArray::~GLVertexArray(){
    glDeleteVertexArrays(1, &array);
}