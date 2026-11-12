//
//  Room.cpp
//  ChatServerCPP
//
//  Created by Anussha on 10/11/26.
//

#include "Room.hpp"
#include <iostream>

void addMember(Room& room, const std::string& name) {
    room.members.push_back(name);
}

bool removeMember(Room& room, const std::string& name) {
    for (auto it = room.members.begin(); it != room.members.end(); ++it) {
        if (*it == name) {
            room.members.erase(it);
            return true;
        }
    }
    return false;
}

bool roomExists(const std::map<std::string, Room>& rooms, const std::string& name) {
    return rooms.find(name) != rooms.end();
}

void printMembers(const Room& room) {
    std::cout << "Room: " << room.name << "\n";
    for (const std::string& name : room.members) {
        std::cout << "  - " << name << "\n";
    }
}

bool sameRoom(const std::map<int, std::string>& clientRoom, int a, int b) {
    auto ia = clientRoom.find(a);  // iterator for client a, or end()
    auto ib = clientRoom.find(b);  // iterator for client b, or end()
    if (ia == clientRoom.end() || ib == clientRoom.end()) {
        // one of them isn't in the map, so not the same room
        return false;
    }
    // a's room name is not blank and it matches b's room name
    return !ia->second.empty() && ia->second == ib->second;
}

bool inRoom(const std::map<int, std::string>& clientRoom, int client) {
    auto it = clientRoom.find(client); // iterator for this client, or end()
    // the client is in the map  and its room name is not blank
    return it != clientRoom.end() && !it->second.empty();
}
