#ifndef __UTILS_RENDER_VERTEXBUFFER__
#define __UTILS_RENDER_VERTEXBUFFER__

#include <define.h>

class Vertex : public Object {

};

class VertexBuffer : public Object {
private:
    uint vertSize;

public:
    VertexBuffer();
    ~VertexBuffer();
};

#endif