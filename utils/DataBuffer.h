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
    DataBuffer(DataBuffer &&);
    DataBuffer(const DataBuffer &);
    DataBuffer &operator=(DataBuffer &&);
    DataBuffer &operator=(const DataBuffer &);
    DataBuffer(size_t len);
    DataBuffer(const void* buf, size_t len);
    ~DataBuffer();

    void Write(char c);
    void Write(const void* buf, size_t len);
    char Read();
    size_t Read(void* buf, size_t len);
    // 设置读取位置
    void Seek(size_t pos);
    // 获取读取位置
    size_t Tell();
    // 获取写入位置
    size_t GetPtr();
    // 设置写入位置
    void SetPtr(size_t pos);
    // 写入数据后记得SetPtr以记录已使用存储
    void* Buffer();
    size_t Size();
};

#endif