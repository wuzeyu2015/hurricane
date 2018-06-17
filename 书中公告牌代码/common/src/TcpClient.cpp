#include "Socket.h"

#include <cstring>

TcpClient::TcpClient() :
    TcpConnection(socket(AF_INET, SOCK_STREAM, 0)) {
}

TcpClient::~TcpClient() {
}

void TcpClient::Connect(const std::string& host, uint16_t port) {
    if ( _socket == -1 ) {
        throw SocketException(errno, "Create TCP server socket error");
    }

    _host = host;
    _port = port;

    Socket::NativeAddress serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));  

    serverAddress.sin_family = AF_INET;  
    serverAddress.sin_addr.s_addr = inet_addr(_host.c_str());  
    serverAddress.sin_port = htons(_port);  

    int err = connect(_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)); 
    if ( err == -1 ) {
        throw SocketException(errno, "Connection to server " + _host + ":" + std::to_string(_port) + " failed");
    }
}
