//
//  main.cpp
//  ChatServerCPP
//
//  Created by Anussha on 10/11/26.
//

#include <iostream>
#include <unistd.h>
#include "Server.hpp"

int main() {
    int server = createServerSocket();
    if (server < 0) {
        return 1;
    }

    std::cout << "Socket created\n";

    if (!startListening(server, 8080)) {
        close(server);
        return 1;
    }

    std::cout << "Waiting for a client on port 8080...\n";
    int client = acceptClient(server);
    if (client < 0) {
        close(server);
        return 1;
    }
    std::cout << "Client connected\n";
    std::string message = readLine(client);
    std::cout << "Client said: " << message << "\n";
    sendLine(client, "Hello from server");
    close(client);
    close(server);
}
