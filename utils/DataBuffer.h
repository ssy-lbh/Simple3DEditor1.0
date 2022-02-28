#ifndef __UTILS_DATABUFFER__
#define __UTILS_DATABUFFER__

#include <define.h>

class DataBuffer final : public Object {
private:
    char* data;
    size_t size;
    size_t ptr;
    size_t rptr = 0;

    void Check(size_t reserve);

public:
    DataBuffer();
    DataBuffer(size_t len);
    DataBuffer(const void* buf, size_t len);
    ~DataBuffer();

    void Write(char c);
    void Write(const void* buf, size_t len);
    char Read();
    size_t Read(void* buf, size_t len);
    void Seek(size_t pos);
    size_t Tell();
    void* Buffer();
    size_t Size();
};

#endif