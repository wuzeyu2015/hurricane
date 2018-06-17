#include "Socket.h"

#include <cstring>

static const int DEFAULT_BACKLOG = 2000;

TcpServer::TcpServer() : 
    Socket(socket(AF_INET, SOCK_STREAM, 0)) {
}

TcpServer::~TcpServer() {
}

void TcpServer::Listen(const std::string& host, uint16_t port, int backlog) {
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

    int err = bind(_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));  
    if ( err == -1 ) {
        throw SocketException(errno, "Bind address: " + host + ":" + std::to_string(port) + " failed");
    }

    if ( !backlog ) {
        backlog = DEFAULT_BACKLOG;
    }
    err = listen(_socket, backlog);  
    if ( err == -1 ) {
        throw SocketException(errno, "Listen address: " + host + ":" + std::to_string(port) + " failed");
    }
}

std::shared_ptr<TcpConnection> TcpServer::Accept() {
    Socket::NativeAddress clientAddress;
    socklen_t addressLength = sizeof(clientAddress);  
    Socket::NativeSocket clientFd = accept(_socket, (struct sockaddr *)&clientAddress, &addressLength);  

    std::shared_ptr<TcpConnection> connection = std::make_shared<TcpConnection>(clientFd);
    connection->SetAddress(clientAddress);

    return connection;
}

