#include "CommandClient.h"
#include "Command.h"
#include "Post.h"

#include <iostream>
#include <thread>
#include <thread>
#include <chrono>

CommandClient::CommandClient(const std::string& host, uint16_t port) :
    _host(host), _port(port), _toExit(false) {
}

CommandClient::~CommandClient() {
}

int CommandClient::Run() {
    try {
        _tcpClient.Connect(_host, _port);
        std::cout << "[INFO] Client is connected to: " << _host << ":" << _port << std::endl;
        
        while ( !_toExit ) {
            std::string commandName;
            std::cout << "> ";
            std::getline(std::cin, commandName);

            if ( commandName == "shutdown" ) {
                DoShutdown();
                Exit();
            }
            else if ( commandName == "exit" ) {
                Exit();
            }
            else if ( commandName == "post") {
                DoPost();
            }
            else if ( commandName == "get" ) {
                DoGet();
            }
            else {
                InvalidCommand(commandName);
            }
        }
    }
    catch ( const SocketException& e ) {
        std::cerr << e.what() << std::endl;

        return e.GetCode();
    }

    return 0;
}

void CommandClient::Exit() {
    _toExit = true;
}

void CommandClient::DoShutdown() {
    Command command;
    command.SetType(Command::Type::ShutdownRequest);

    command.Write(&_tcpClient);
}

void CommandClient::DoPost() {
    std::cout << "Title?: ";
    std::string title;
    std::getline(std::cin, title);

    std::cout << "Content?: ";
    std::string content;
    std::getline(std::cin, content);

    Post newPost(title, content);
    Command command;
    command.SetType(Command::Type::PostRequest);
    command.SetContent(newPost.ToMessage());

    command.Write(&_tcpClient);

    Command response;
    response.Read(&_tcpClient);

    if ( response.GetType() == Command::Type::PostResponse ) {
        std::cout << "[INFO] Create new post successfully" << std::endl;
    }
    else {
        InvalidCommand(response.GetType());
    }
}

void CommandClient::DoGet() {
    Command command;
    command.SetType(Command::Type::GetRequest);

    command.Write(&_tcpClient);

    Command response;
    response.Read(&_tcpClient);

    if ( response.GetType() != Command::Type::GetResponse ) {
        InvalidCommand(response.GetType());
    }

    std::cout << "[INFO] Got latest post" << std::endl;
    std::string responseContent = response.GetContent();
    if ( responseContent.empty() ) {
        std::cerr << "[INFO] There is no any posts now" << std::endl;

        return;
    }

    Post latestPost = Post::FromMessage(responseContent);
    if ( !latestPost.isValid() ) {
        std::cerr << "[ERROR] Response format of GET is invalid" << std::endl;

        return;
    }

    std::cout << "Title: " << latestPost.GetTitle() << std::endl;
    std::cout << "Content: " << latestPost.GetContent() << std::endl;
}

void CommandClient::InvalidCommand(int type) {
    std::cerr << "[ERROR] Command " << type << " is invalid." << std::endl;
}

void CommandClient::InvalidCommand(const std::string& commandName) {
    std::cerr << "[ERROR] Command " << commandName << " is invalid." << std::endl;
}
