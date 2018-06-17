#include "Socket.h"

#include <unistd.h>

Socket::Socket(Socket::NativeSocket socket) :
    _socket(socket) {
}

Socket::~Socket() {
    Close();
}

void Socket::Close() {
    close(_socket);
}
