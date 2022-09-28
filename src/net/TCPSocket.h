#ifndef __NET_TCPSOCKET__
#define __NET_TCPSOCKET__

#include <define.h>

#include <net/Socket.h>

namespace simple3deditor {

class TCPSocket : public Object, public ISocket {
private:
    SOCKET s;

public:
    TCPSocket();
    TCPSocket(const String& addr, int port);

    virtual size_t Read(void* buf, size_t len) override;
    virtual size_t Write(const void* buf, size_t len) override;
    virtual size_t Available() override;
    virtual int Connect(const String& addr, int port) override;
    virtual int Bind(const String& addr, int port) override;
    virtual int Listen(int num) override;
    virtual void Close() override;
    virtual bool IsClosed() override;
};

}

#endif