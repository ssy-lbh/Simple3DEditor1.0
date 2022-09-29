#include <util/render/VertexBuffer.h>

#include <glew/gl/glew.h>

namespace simple3deditor {

VertexBuffer::VertexBuffer(uint vertSize) : vertSize(vertSize) {
    glGenVertexArrays(1, &array);
    glGenBuffers(1, &buffer);
}

VertexBuffer::~VertexBuffer(){
    glDeleteVertexArrays(1, &array);
    glDeleteBuffers(1, &buffer);
}

uint VertexBuffer::GetVertexSize(){
    return vertSize;
}

uenum VertexBuffer::GetMode(Topology topology){
    switch (topology){
    case Topology::TRIANGLES: return GL_TRIANGLES;
    case Topology::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
    case Topology::LINES: return GL_LINES;
    case Topology::POINTS: return GL_POINTS;
    default: return GL_NONE;
    }
}

void VertexBuffer::SetData(Vector4* data, size_t size){
    this->dataSize = (uint)size;

    glBindVertexArray(array);
    // 这里绑定ArrayBuffer可能是一个VertexArray的内部对象
    // 是设置已经绑定的VertexArray状态的语句
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vector4), data, GL_STATIC_DRAW);

    for (uint i = 0; i < vertSize; i++){
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, vertSize * sizeof(Vector4), (void*)(i * sizeof(Vector4)));
    }

    glBindVertexArray(0);
}

void VertexBuffer::SetTopology(Topology topology){
    uint vertCount = dataSize / vertSize;

    this->topology = topology;
    switch (topology){
    case Topology::TRIANGLES: count = vertCount / 3; break;
    case Topology::TRIANGLE_STRIP: count = vertCount - 2; break;
    case Topology::LINES: count = vertCount >> 1; break;
    case Topology::POINTS: count = vertCount; break;
    default: count = 0; break;
    }
}

void VertexBuffer::Draw(Topology topology, uint start, uint count){
    glBindVertexArray(array);
    glDrawArrays((uenum)topology, start, count);
}

void VertexBuffer::DrawAll(){
    glBindVertexArray(array);
    glDrawArrays((uenum)this->topology, 0, count);
}

void VertexBuffer::DrawAll(Topology topology){
    SetTopology(topology);
    glBindVertexArray(array);
    glDrawArrays((uenum)topology, 0, count);
}

}