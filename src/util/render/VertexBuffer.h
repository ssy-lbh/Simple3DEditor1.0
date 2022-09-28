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

    // 暂时弃用
    uenum GetMode(Topology topology);

public:
    // 默认Vector4个数，以后改进
    VertexBuffer(uint vertSize);
    ~VertexBuffer();

    // 顶点数据所占用Vertex4个数
    uint GetVertexSize();
    // 直接填入buffer，size为Vector4个数
    void SetData(Vector4* data, size_t size);
    void SetTopology(Topology topology);
    void Draw(Topology topology, uint start, uint count);
    // 需要先SetTopology
    void DrawAll();
    void DrawAll(Topology topology);
};

}

#endif