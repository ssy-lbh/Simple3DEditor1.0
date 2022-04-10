#include <utils/render/BufferBuilder.h>

BufferBuilder::BufferBuilder(){}
BufferBuilder::~BufferBuilder(){}

void BufferBuilder::BindBuffer(VertexBuffer* buffer){
    vertSize = buffer->GetVertexSize();
    this->buffer = buffer;
}

void BufferBuilder::Finish(){
    buffer->SetData(data.GetData(), data.Size());
    buffer = NULL;
}

void BufferBuilder::Emit(){
    switch (vertSize){
    case 0: break;
    case 1: data.Add(pos); break;
    case 2: data.Add(pos); data.Add(uv); break;
    case 3: data.Add(pos); data.Add(uv); data.Add(normal); break;
    default:
        data.Add(pos);
        data.Add(uv);
        data.Add(normal);
        for (size_t i = 3; i < vertSize; i++)
            data.Add(Vector4::zero);
        break;
    }
}

void BufferBuilder::Vertex(Point2 pos){
    this->pos = pos;
    Emit();
}

void BufferBuilder::Vertex(Point3 pos){
    this->pos = pos;
    Emit();
}

void BufferBuilder::Vertex(Vector4 pos){
    this->pos = pos;
    Emit();
}

void BufferBuilder::TexCoord(Vector2 uv){
    this->uv = uv;
}

void BufferBuilder::Normal(Vector3 normal){
    this->normal = normal;
}