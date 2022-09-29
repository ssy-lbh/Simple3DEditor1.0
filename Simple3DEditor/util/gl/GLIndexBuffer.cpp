#include <util/gl/GLIndexBuffer.h>

#include <lib/glew/gl/glew.h>

#include <util/os/Log.h>

namespace simple3deditor {

GLIndexBuffer::GLIndexBuffer(){
    glGenBuffers(1, &buffer);
}

GLIndexBuffer::~GLIndexBuffer(){
    glDeleteBuffers(1, &buffer);
}

}