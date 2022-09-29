#ifndef __UTIL_RENDER_VERTEXBUFFER__
#define __UTIL_RENDER_VERTEXBUFFER__

#include <define.h>

#include <lib/glew/gl/glew.h>

#include <util/math3d/LinearAlgebra.h>

namespace simple3deditor {

enum class Topology {
    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    POINTS = GL_POINTS,
    LINES = GL_LINES
};

class VertexBuffer : public Object {
private:
    uint array;
    uint buffer;
    uint vertSize;

    uint dataSize;
    Topology topology = Topology::TRIANGLES;
    uint count;

    // ��ʱ����
    uenum GetMode(Topology topology);

public:
    // Ĭ��Vector4�������Ժ�Ľ�
    VertexBuffer(uint vertSize);
    ~VertexBuffer();

    // ����������ռ��Vertex4����
    uint GetVertexSize();
    // ֱ������buffer��sizeΪVector4����
    void SetData(Vector4* data, size_t size);
    void SetTopology(Topology topology);
    void Draw(Topology topology, uint start, uint count);
    // ��Ҫ��SetTopology
    void DrawAll();
    void DrawAll(Topology topology);
};

}

#endif