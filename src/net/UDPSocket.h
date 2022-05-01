#ifndef __NET_UDPSOCKET__
#define __NET_UDPSOCKET__

#include <define.h>

#include <net/Socket.h>

class UDPSocket : public Object, public ISocket {
private:
    SOCKET s;

public:
    UDPSocket();
    UDPSocket(const String& addr, int port);

    virtual size_t Read(void* buf, size_t len) override;
    virtual size_t Write(const void* buf, size_t len) override;
    virtual size_t Available() override;
    virtual int Connect(const String& addr, int port) override;
    virtual int Bind(const String& addr, int port) override;
    virtual int Listen(int num) override;
    virtual void Close() override;
    virtual bool IsClosed() override;
};

#endif