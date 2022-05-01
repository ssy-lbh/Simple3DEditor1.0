#include <util/gl/GLVertexBuffer.h>

#include <lib/glew/gl/glew.h>

#include <util/os/Log.h>

GLVertexBuffer::GLVertexBuffer(){
    glGenBuffers(1, &buffer);
}

GLVertexBuffer::~GLVertexBuffer(){
    glDeleteBuffers(1, &buffer);
}

uint GLVertexBuffer::GetBuffer(){
    return buffer;
}