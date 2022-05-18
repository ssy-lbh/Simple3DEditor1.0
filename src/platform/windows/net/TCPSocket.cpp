#include <net/TCPSocket.h>

#include <winsock2.h>

namespace simple3deditor {

TCPSocket::TCPSocket(){
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

TCPSocket::TCPSocket(const String& addr, int port){
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    Connect(addr, port);
}

size_t TCPSocket::Read(void* buf, size_t len) {
    return recv(s, (char*)buf, len, 0);
}

size_t TCPSocket::Write(const void* buf, size_t len) {
    return send(s, (const char*)buf, len, 0);
}

size_t TCPSocket::Available() {
    u_long len;
    ioctlsocket(s, FIONREAD, &len);
    return (size_t)len;
}

int TCPSocket::Connect(const String& addr, int port) {
    SOCKADDR_IN addrIn;
    addrIn.sin_family = AF_INET;
    addrIn.sin_addr.S_un.S_addr = inet_addr(addr.GetString());
    addrIn.sin_port = htons((u_short)port);
    return connect(s, (PSOCKADDR)&addrIn, sizeof(addrIn));
}

int TCPSocket::Bind(const String& addr, int port) {
    SOCKADDR_IN addrIn;
    addrIn.sin_family = AF_INET;
    addrIn.sin_addr.S_un.S_addr = inet_addr(addr.GetString());
    addrIn.sin_port = htons((u_short)port);
    return bind(s, (PSOCKADDR)&addrIn, sizeof(addrIn));
}

int TCPSocket::Listen(int num){
    return listen(s, num);
}

void TCPSocket::Close(){
    closesocket(s);
    s = -1;
}

bool TCPSocket::IsClosed(){
    return s == -1;
}

}