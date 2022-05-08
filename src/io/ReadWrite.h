#ifndef __IO_READWRITE__
#define __IO_READWRITE__

#include <define.h>

#include <base/Closeable.h>
#include <base/Readable.h>
#include <base/Flushable.h>
#include <base/Writable.h>
#include <base/Appendable.h>

class AReader : public IReadable, public ICloseable {
protected:
    volatile bool lock = false;

    AReader() {}

public:
    using IReadable::Read;
    virtual size_t Read(char* s, size_t size) = 0;
    virtual bool Ready();
};

class AWriter : public IAppendable, public ICloseable, public IFlushable {
protected:
    volatile bool lock = false;

    AWriter() {}

public:
    using IAppendable::Append;
    virtual void Write(char c);
    virtual void Write(const char* s, size_t size) = 0;
    virtual void Write(const char* s);
    virtual AWriter& Append(const char* s) override;
    virtual AWriter& Append(const char* s, size_t size) override;
    virtual AWriter& Append(char c) override;
};

#endif