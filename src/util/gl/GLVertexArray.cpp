#include <util/gl/GLVertexArray.h>

#include <glew/gl/glew.h>

#include <util/gl/GLVertexBuffer.h>

namespace simple3deditor {

GLVertexArray::GLVertexArray(){
    glGenVertexArrays(1, &array);
}

GLVertexArray::~GLVertexArray(){
    glDeleteVertexArrays(1, &array);
}

void GLVertexArray::BindBuffer(GLVertexBuffer* buffer){
    glBindVertexArray(array);
    glBindBuffer(GL_ARRAY_BUFFER, (buffer == NULL ? 0 : buffer->GetBuffer()));
    glBindVertexArray(0);
}

void GLVertexArray::SetFormat(uint idx, uint size, uenum type, uint offset){
    glBindVertexArray(array);
    glVertexAttribFormat(idx, size, type, GL_FALSE, offset);
    glBindVertexArray(0);
}

uint GLVertexArray::GetArray(){
    return array;
}

}