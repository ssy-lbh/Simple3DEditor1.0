#ifndef __IO_IOSTREAM__
#define __IO_IOSTREAM__

#include <define.h>

#include <base/Closeable.h>
#include <base/Flushable.h>
#include <base/Readable.h>
#include <base/Writable.h>

class IInputStream : public IReadable, public ICloseable, public IFlushable {
public:
    virtual size_t Read(void* buf, size_t len) = 0;
    virtual size_t Available();
    virtual void Flush();
    virtual void Close();
};

class IOutputStream : public IWritable, public ICloseable, public IFlushable {
public:
    virtual size_t Write(const void* buf, size_t len) = 0;
    virtual void Flush();
    virtual void Close();
};

#endif