//
//  main.cpp
//  ChatServerCPP
//
//  Created by Anussha on 10/11/26.
//

#include "Server.hpp"
#include <iostream>

int main() {
    int server = createServerSocket();
    if (server < 0) {
        return 1;
    }

    if (!startListening(server, 8080)) {
        return 1;
    }

    std::cout << "server listening on port 8080\n";
    runServer(server);

    return 0;
}
