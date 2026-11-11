//
//  Server.hpp
//  ChatServerCPP
//
//  Created by Anussha on 10/11/26.
//

#ifndef Server_hpp
#define Server_hpp

#include <stdio.h>
#include <iostream>
#include <poll.h>
#include <vector>
#pragma once

extern std::vector<pollfd> fds; // every socket the server is watching
int createServerSocket();
bool startListening(int server, int port);
int acceptClient(int server);
std::string readLine(int client);
bool sendLine(int client, const std::string& text);
void runServer(int server);
#endif /* Server_hpp */
