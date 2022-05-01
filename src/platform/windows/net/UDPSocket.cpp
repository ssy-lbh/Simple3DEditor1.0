#include <net/UDPSocket.h>

#include <winsock2.h>

UDPSocket::UDPSocket(){
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

UDPSocket::UDPSocket(const String& addr, int port){
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    Connect(addr, port);
}

size_t UDPSocket::Read(void* buf, size_t len) {
    return recv(s, (char*)buf, len, 0);
}

size_t UDPSocket::Write(const void* buf, size_t len) {
    return send(s, (const char*)buf, len, 0);
}

size_t UDPSocket::Available() {
    u_long len;
    ioctlsocket(s, FIONREAD, &len);
    return (size_t)len;
}

int UDPSocket::Connect(const String& addr, int port) {
    SOCKADDR_IN addrIn;
    addrIn.sin_family = AF_INET;
    addrIn.sin_addr.S_un.S_addr = inet_addr(addr.GetString());
    addrIn.sin_port = htons((u_short)port);
    return connect(s, (PSOCKADDR)&addrIn, sizeof(addrIn));
}

int UDPSocket::Bind(const String& addr, int port) {
    SOCKADDR_IN addrIn;
    addrIn.sin_family = AF_INET;
    addrIn.sin_addr.S_un.S_addr = inet_addr(addr.GetString());
    addrIn.sin_port = htons((u_short)port);
    return bind(s, (PSOCKADDR)&addrIn, sizeof(addrIn));
}

int UDPSocket::Listen(int num){
    return listen(s, num);
}

void UDPSocket::Close(){
    closesocket(s);
    s = -1;
}

bool UDPSocket::IsClosed(){
    return s == -1;
}