#ifndef __UTILS_RENDER_BUFFERBUILDER__
#define __UTILS_RENDER_BUFFERBUILDER__

#include <define.h>

#include <util/List.h>
#include <util/math3d/LinearAlgebra.h>
#include <util/render/VertexBuffer.h>

// 位置放在[0]，UV放在[1]，法线放在[2]
class BufferBuilder : public Object {
private:
    VertexBuffer* buffer = NULL;
    uint vertSize;

    List<Vector4> data;

    Vector4 pos = Vector4::zero;
    Vector4 uv = Vector4::zero;
    Vector4 normal = Vector4::zero;

public:
    BufferBuilder();
    ~BufferBuilder();

    // 不能为NULL
    void BindBuffer(VertexBuffer* buffer);
    void Finish();
    void Emit();
    void Vertex(Point2 pos);
    void Vertex(Point3 pos);
    void Vertex(Vector4 pos);
    void TexCoord(Vector2 uv);
    void Normal(Vector3 normal);
};

#endif