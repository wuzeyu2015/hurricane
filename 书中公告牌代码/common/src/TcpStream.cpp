#include "Socket.h"

#include <sys/types.h>
#include <sys/socket.h>

TcpStream::TcpStream(Socket::NativeSocket socket) :
    Socket(socket) {
}

TcpStream::~TcpStream() {
}

ssize_t TcpStream::Send(const void* buffer, size_t len) {
    const char* byteBuffer = reinterpret_cast<const char*>(buffer);
    ssize_t err = send(_socket, byteBuffer, len, 0);
    if ( err == -1 ) {
        throw SocketException(errno, "Socket send error");
    }

    if ( err < 0 ) {
        throw SocketException(1, "Unkown error");
    }

    size_t sentBytes = static_cast<size_t>(err);
    size_t totalSent = sentBytes;
    while ( sentBytes < len && sentBytes != 0 ) {
        byteBuffer += sentBytes;

        len -= sentBytes;
        err = send(_socket, byteBuffer, len, 0);
        if ( err == -1 ) {
            throw SocketException(errno, "Socket send error");
        }

        if ( err < 0 ) {
            throw SocketException(1, "Unkown error");
        }

        sentBytes = static_cast<size_t>(err);
        totalSent += sentBytes;
    }

    return totalSent;
}

ssize_t TcpStream::Receive(void* buffer, size_t len) {
    ssize_t err = recv(_socket, buffer, len, 0);
    if ( err == -1 ) {
        throw SocketException(errno, "Socket receive error");
    }

    return err;
}

ssize_t TcpStream::Write(const void* buffer, size_t len) {
    return Send(buffer, len);
}

ssize_t TcpStream::Read(void* buffer, size_t len) {
    return Receive(buffer, len);
}
