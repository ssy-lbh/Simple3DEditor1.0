#ifndef __UTILS_RENDER_BUFFERBUILDER__
#define __UTILS_RENDER_BUFFERBUILDER__

#include <define.h>

#include <utils/render/VertexBuffer.h>

class BufferBuilder : public Object {
private:
    VertexBuffer* buffer = NULL;

public:
    BufferBuilder();
    ~BufferBuilder();

    void BindBuffer(VertexBuffer* buffer);
    
};

#endif