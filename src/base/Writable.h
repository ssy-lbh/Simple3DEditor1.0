#ifndef __BASE_WRITABLE__
#define __BASE_WRITABLE__

#include <define.h>

namespace simple3deditor {

class IWritable {
public:
    virtual void Write(char c);
    virtual void Write(wchar_t c);
    virtual size_t Write(const void* buf, size_t len) = 0;
    virtual void Write(bool b);
    virtual void Write(short s);
    virtual void Write(int i);
    virtual void Write(long long l);
    virtual void Write(float f);
    virtual void Write(double d);
    virtual void Write(const char* s);
    virtual void Write(const wchar_t* s);
    virtual void WriteWithLen(const char* s);
    virtual void WriteWithLen(const wchar_t* s);
    virtual void WriteWithLen(const char* s, int len);
    virtual void WriteWithLen(const wchar_t* s, int len);
    virtual void WriteWithLen(const String& s);
    virtual void WriteWithLen(const WString& s);
};

}

#endif