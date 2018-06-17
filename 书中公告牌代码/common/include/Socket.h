#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>

#include <string>
#include <cstdint>
#include <memory>
#include <exception>

class SocketException : public std::exception {
public:
    SocketException(int code, const std::string& message);

    const char* what() const noexcept;

    int GetCode() const;
    const std::string& GetMessage() const;

private:
    int _code;
    std::string _message;
    std::string _what;
};

class Socket {
public:
    using NativeSocket = int;
    using NativeAddress = struct sockaddr_in;

    Socket(NativeSocket socket);
    virtual ~Socket();

    void Close();

protected:
    NativeSocket _socket;
};

class TcpStream : public Socket {
public:
    TcpStream(Socket::NativeSocket socket);
    ~TcpStream();

    ssize_t Send(const void* buffer, size_t len);
    ssize_t Receive(void* buffer, size_t len);

    ssize_t Write(const void* buffer, size_t len);
    ssize_t Read(void* buffer, size_t len);
};

class TcpConnection: public TcpStream {
public:
    TcpConnection(Socket::NativeSocket socket);
    ~TcpConnection();

    const std::string& GetHost() const;
    uint16_t GetPort() const;
    void SetAddress(const Socket::NativeAddress& address);

private:
    std::string _host;
    uint16_t _port;
};

class TcpServer : public Socket {
public:
    TcpServer();
    ~TcpServer();

    void Listen(const std::string& host, uint16_t port, int backlog = 0);
    std::shared_ptr<TcpConnection> Accept();

private:
    std::string _host;
    uint16_t _port;
};

class TcpClient : public TcpConnection {
public:
    TcpClient();
    ~TcpClient();

    void Connect(const std::string& host, uint16_t port);

private:
    std::string _host;
    uint16_t _port;
};
