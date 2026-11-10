//
//  Server.cpp
//  ChatServerCPP
//
//  Created by Anussha on 10/11/26.
//

#include "Server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int createServerSocket() {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        std::cerr << "socket failed\n";
        return -1;
    }
    return server;
}

bool startListening(int server, int port) {
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(server, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
        std::cerr << "bind failed\n";
        return false;
    }
    if (listen(server, 5) < 0) {
        std::cerr << "listen failed\n";
        return false;
    }
    return true;
}

int acceptClient(int server) {
    int client = accept(server, nullptr, nullptr);
    if (client < 0) {
        std::cerr << "accept failed\n";
    }
    return client;
}

std::string readLine(int client) {
    std::string line;
    char ch;
    while (read(client, &ch, 1) == 1) {
        if (ch == '\n') {
            break;
        }
        line += ch;
    }
    return line;
}

bool sendLine(int client, const std::string& text) {
    std::string data = text + "\n";
    ssize_t sent = write(client, data.c_str(), data.size());
    return sent == static_cast<ssize_t>(data.size());
}
