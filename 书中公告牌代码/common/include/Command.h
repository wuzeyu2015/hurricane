#pragma once

#include <string>
#include <cstdint>

class TcpStream;

class Command {
public:
    class Type {
    public:
        enum {
            Invalid = 0,
            PostRequest = 1,
            GetRequest = 2,
            PostResponse = 3,
            GetResponse = 4,
            ShutdownRequest = 5
        };
    };

    Command();
    virtual ~Command();

    int32_t GetVersion() const;
    int32_t GetType() const;
    const std::string& GetContent() const;

    void SetVersion(int32_t version);
    void SetType(int32_t type);
    void SetContent(const std::string& content);

    virtual void Read(TcpStream* stream);
    virtual void Write(TcpStream* stream);

private:
    int32_t _version;
    int32_t _type;
    std::string _content;
};
