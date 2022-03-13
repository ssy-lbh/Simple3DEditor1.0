#include <utils/gl/GLVertexBuffer.h>

#include <lib/glew/gl/glew.h>

#include <utils/os/Log.h>

GLVertexBuffer::GLVertexBuffer(){
    glGenBuffers(1, &buffer);
}

GLVertexBuffer::~GLVertexBuffer(){
    glDeleteBuffers(1, &buffer);
}

uint GLVertexBuffer::GetBuffer(){
    return buffer;
}