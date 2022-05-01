#ifndef __BASE_WRITABLE__
#define __BASE_WRITABLE__

#include <define.h>

class IWritable {
public:
    virtual void Write(char c);
    virtual size_t Write(const void* buf, size_t len) = 0;
    virtual void Write(short s);
    virtual void Write(int i);
    virtual void Write(long long l);
    virtual void Write(float f);
    virtual void Write(double d);
};

#endif