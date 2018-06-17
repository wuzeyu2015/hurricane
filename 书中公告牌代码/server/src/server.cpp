#include "CommandServer.h"

#include <iostream>
#include <cstring>

int main() {
    CommandServer server("127.0.0.1", 8088);

    return server.Run();
}
