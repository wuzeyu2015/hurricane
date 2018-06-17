#pragma once

#include <string>

class Post {
public:
    static Post FromMessage(const std::string& message) {
        size_t pos = message.find("\n");
        if ( pos == std::string::npos ) {
            return Post();
        }

        std::string title = message.substr(0, pos);
        std::string content = message.substr(pos);

        return Post(title, content);
    }

    Post() : _valid(false) {
    }

    Post(const std::string& title, const std::string& content) :
        _valid(true), _title(title), _content(content) {
    }

    const std::string ToMessage() const {
        return _title + "\n" + _content;
    }

    bool isValid() const {
        return _valid;
    }

    void SetValid(bool valid) {
        _valid = valid;
    }

    const std::string& GetTitle() const {
        return _title;
    }

    void SetTile(const std::string& title) {
        _title = title;
    }

    const std::string& GetContent() const {
        return _content;
    }

    void SetContent(const std::string& content) {
        _content = content;
    }
    
private:
    bool _valid;
    std::string _title;
    std::string _content;
};

