#include "Command.h"
#include "Socket.h"

Command::Command() :
    _version(1), _type(Command::Type::Invalid) {
}

Command::~Command() {
}

int32_t Command::GetVersion() const {
    return _version;
}

int32_t Command::GetType() const {
    return _type;
}

const std::string& Command::GetContent() const {
    return _content;
}

void Command::SetVersion(int32_t version) {
    _version = version;
}

void Command::SetType(int32_t type) {
    _type = type;
}

void Command::SetContent(const std::string& content) {
    _content = content;
}

void Command::Read(TcpStream* stream) {
    int32_t size;

    stream->Read(&_version, sizeof(_version));
    stream->Read(&_type, sizeof(_type));
    stream->Read(&size, sizeof(size));

    if ( size ) {
        char* buffer = new char[size];
        stream->Read(buffer, size * sizeof(char));
        _content = std::string(buffer, size);

        delete [] buffer;
    }
}

void Command::Write(TcpStream* stream) {
    int32_t size = _content.size();

    stream->Write(&_version, sizeof(_version));
    stream->Write(&_type, sizeof(_type));
    stream->Write(&size, sizeof(size));

    if ( size ) {
        stream->Write(_content.c_str(), size * sizeof(char));
    }
}
