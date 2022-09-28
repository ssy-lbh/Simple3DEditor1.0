#ifndef __NET_SOCKET__
#define __NET_SOCKET__

#include <define.h>

#include <base/Closeable.h>
#include <io/IOStream.h>
#include <util/String.h>

namespace simple3deditor {

#ifdef PLATFORM_WINDOWS64
typedef unsigned long long SOCKET;
#endif
#ifdef PLATFORM_WINDOWS32
typedef unsigned int SOCKET;
#endif

class ISocket : public IInputStream, public IOutputStream {
public:
    virtual size_t Read(void* buf, size_t len) = 0;
    virtual size_t Write(const void* buf, size_t len) = 0;
    virtual size_t Available() = 0;
    virtual int Connect(const String& addr, int port) = 0;
    virtual int Bind(const String& addr, int port) = 0;
    virtual int Listen(int num) = 0;
    virtual void Close() = 0;
    virtual bool IsClosed() = 0;
};

}

#endif