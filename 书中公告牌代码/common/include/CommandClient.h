#pragma once

#include "Socket.h"
#include <cstdint>

class CommandClient {
public:
    CommandClient(const std::string& host, uint16_t port);
    ~CommandClient();

    int Run();

private:
    void Exit();
    void DoShutdown();
    void DoPost();
    void DoGet();
    void InvalidCommand(int type);
    void InvalidCommand(const std::string& commandName);

private:
    TcpClient _tcpClient;
    std::string _host;
    uint16_t _port;
    bool _toExit;
};

