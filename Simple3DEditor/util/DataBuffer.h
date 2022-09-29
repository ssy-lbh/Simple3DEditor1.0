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
    // const DataBuffer ��������commit = trueʱ���й�buf�ڴ�ָ��
    DataBuffer(const void* buf, size_t len, bool commit);
    virtual ~DataBuffer();

    void Write(char c);
    void Write(const void* buf, size_t len);
    char Read() const;
    size_t Read(void* buf, size_t len) const;

    // ���ö�ȡλ��
    inline void Seek(size_t pos) const{
        rptr = pos;
    }

    // ��ȡ��ȡλ��
    inline size_t Tell() const{
        return rptr;
    }

    // ��ȡд��λ��
    inline size_t GetPtr() const{
        return ptr;
    }

    // ����д��λ��
    inline void SetPtr(size_t pos){
        ptr = pos;
    }

    // д�����ݺ�ǵ�SetPtr�Լ�¼��ʹ�ô洢
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

// const DataBuffer������
class PackDataBuffer : public DataBuffer {
public:
    // ȷ��PackDataBuffer����֮ǰDataBuffer�������
    // �����ڴ��ݲ���
    PackDataBuffer(DataBuffer &&);
    PackDataBuffer(const DataBuffer &);
    PackDataBuffer(const void* data, size_t size);
    virtual ~PackDataBuffer() override;
};

}

#endif