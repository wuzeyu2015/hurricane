#include "CommandServer.h"
#include "Command.h"

#include <iostream>

CommandServer::CommandServer(const std::string& host, uint16_t port) :
    _host(host), _port(port), _toExit(false) {
}

CommandServer::~CommandServer() {
}

int CommandServer::Run() {
    try {
        _tcpServer.Listen(_host, _port);
        std::cout << "[INFO] Server is listening on: " << _host << ":" << _port << std::endl;
        
        while ( !_toExit ) {
            std::shared_ptr<TcpConnection> connection = _tcpServer.Accept();
            std::cout << "[INFO] Accepted client from: " << connection->GetHost() << ":" << connection->GetPort() << std::endl;

            bool clientAlived = true;
            while ( clientAlived ) {
                Command command;
                command.Read(connection.get());

                int commandType = command.GetType();
                switch ( commandType ) {
                case Command::Type::Invalid:
                    InvalidCommand(commandType);
                    clientAlived = false;
                    break;
                case Command::Type::ShutdownRequest:
                    Shutdown();
                    clientAlived = false;
                    break;
                case Command::Type::PostRequest: {
                    Command response = OnPost(command);
                    response.Write(connection.get());

                    break;
                }
                case Command::Type::GetRequest: {
                    Command response = OnGet(command);
                    response.Write(connection.get());

                    break;
                }
                default:
                    InvalidCommand(commandType);
                    clientAlived = false;
                    break;
                }
            }

            std::cout << "[INFO] Client disconneted: " << connection->GetHost() << ":" << connection->GetPort() << std::endl;
        }
    }
    catch ( const SocketException& e ) {
        std::cerr << e.what() << std::endl;

        return e.GetCode();
    }

    return 0;
}

Command CommandServer::OnPost(const Command& command) {
    std::cout << "[INFO] POST" << std::endl;

    Command response;
    response.SetType(Command::Type::PostResponse);

    Post newPost = Post::FromMessage(command.GetContent());
    if ( !newPost.isValid() ) {
        std::cerr << "[ERROR] The format of new post is invalid" << std::endl;
        response.SetContent("error");

        return response;
    }

    _posts.push_back(newPost);
    response.SetContent("ok");

    return response;
}

Command CommandServer::OnGet(const Command& command) {
    std::cout << "[INFO] GET" << std::endl;

    Command response;
    response.SetType(Command::Type::GetResponse);

    if ( !_posts.size() ) {
        return response;
    }

    Post latestPost = _posts.back();
    response.SetContent(latestPost.ToMessage());

    return response;
}

void CommandServer::Shutdown() {
    std::cout << "[INFO] Shutdown server by remote" << std::endl;
    _toExit = true;
}

void CommandServer::InvalidCommand(int type) {
    std::cerr << "[ERROR] Command " << type << " is invalid." << std::endl;
}
