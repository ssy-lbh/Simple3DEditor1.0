#include <util/gl/GLVertexBuffer.h>

#include <glew/gl/glew.h>

#include <util/os/Log.h>

namespace simple3deditor {

GLVertexBuffer::GLVertexBuffer(){
    glGenBuffers(1, &buffer);
}

GLVertexBuffer::~GLVertexBuffer(){
    glDeleteBuffers(1, &buffer);
}

uint GLVertexBuffer::GetBuffer(){
    return buffer;
}

}