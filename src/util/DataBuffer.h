#ifndef __UTIL_DATABUFFER__
#define __UTIL_DATABUFFER__

#include <define.h>

namespace simple3deditor {

class DataBuffer : public Object {
protected:
    char* data;
    size_t size;
    size_t ptr;
    mutable size_t rptr = 0;

    inline DataBuffer(bool alloc){
        if (alloc){
            data = new char[32];
            size = 32;
            ptr = 0;
        }
    }

    void Check(size_t reserve);

public:
    DataBuffer();
    DataBuffer(DataBuffer &&);
    DataBuffer(const DataBuffer &);
    DataBuffer &operator=(DataBuffer &&);
    DataBuffer &operator=(const DataBuffer &);
    DataBuffer(size_t len);
    DataBuffer(const void* buf, size_t len);
    // const DataBuffer 构造器，commit = true时，托管buf内存指针
    DataBuffer(const void* buf, size_t len, bool commit);
    virtual ~DataBuffer();

    void Write(char c);
    void Write(const void* buf, size_t len);
    char Read() const;
    size_t Read(void* buf, size_t len) const;

    // 设置读取位置
    inline void Seek(size_t pos) const{
        rptr = pos;
    }

    // 获取读取位置
    inline size_t Tell() const{
        return rptr;
    }

    // 获取写入位置
    inline size_t GetPtr() const{
        return ptr;
    }

    // 设置写入位置
    inline void SetPtr(size_t pos){
        ptr = pos;
    }

    // 写入数据后记得SetPtr以记录已使用存储
    inline void* Buffer(){
        return data;
    }

    inline const void* ReadOnlyBuffer() const{
        return data;
    }

    inline size_t Size() const{
        return ptr;
    }
};

// const DataBuffer构造类
class PackDataBuffer : public DataBuffer {
public:
    // 确保PackDataBuffer回收之前DataBuffer不会回收
    // 可用于传递参数
    PackDataBuffer(DataBuffer &&);
    PackDataBuffer(const DataBuffer &);
    PackDataBuffer(const void* data, size_t size);
    virtual ~PackDataBuffer() override;
};

}

#endif