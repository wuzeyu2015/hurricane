#include "CommandClient.h"

#include <iostream>

int main() {
    CommandClient client("127.0.0.1", 8088);

    return client.Run();
}
