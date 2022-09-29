#ifndef __BASE_READABLE__
#define __BASE_READABLE__

#include <define.h>

namespace simple3deditor {

class IReadable {
private:
    static constexpr size_t SKIP_BUFFER_SIZE = 2048;
    static char skipBuffer[SKIP_BUFFER_SIZE];

public:
    virtual int Read();
    virtual size_t Read(void* buf, size_t len) = 0;
    virtual bool ReadBool();
    virtual short ReadShort();
    virtual int ReadInt();
    virtual long long ReadLong();
    virtual float ReadFloat();
    virtual double ReadDouble();
    virtual size_t Skip(size_t n);
    // 注意，此函数对应IWritable::WriteWithLen函数
    virtual String ReadString();
    // 注意，此函数对应IWritable::WriteWithLen函数
    virtual WString ReadWString();
};

}

#endif