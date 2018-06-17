#pragma once

#include "Socket.h"
#include "Post.h"
#include <cstdint>
#include <vector>

class Command;

class CommandServer {
public:
    CommandServer(const std::string& host, uint16_t port);
    ~CommandServer();

    int Run();

private:
    Command OnPost(const Command& command);
    Command OnGet(const Command& command);
    void Shutdown();
    void InvalidCommand(int type);

private:
    TcpServer _tcpServer;
    std::string _host;
    uint16_t _port;
    bool _toExit;
    std::vector<Post> _posts;
};

