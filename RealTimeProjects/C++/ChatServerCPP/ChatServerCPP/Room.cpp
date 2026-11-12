//
//  Room.cpp
//  ChatServerCPP
//
//  Created by Anussha on 10/11/26.
//



#include "Room.hpp"

bool sameRoom(const std::map<int, std::string>& clientRoom, int a, int b) {
    auto ia = clientRoom.find(a);
    auto ib = clientRoom.find(b);
    if (ia == clientRoom.end() || ib == clientRoom.end()) {
        return false;
    }
    return !ia->second.empty() && ia->second == ib->second;
}

bool inRoom(const std::map<int, std::string>& clientRoom, int client) {
    auto it = clientRoom.find(client);
    return it != clientRoom.end() && !it->second.empty();
}

void joinRoom(std::map<int, std::string>& clientRoom, int client, const std::string& room) {
    clientRoom[client] = room;
}

void leaveRoom(std::map<int, std::string>& clientRoom, int client) {
    clientRoom[client].clear();
}
