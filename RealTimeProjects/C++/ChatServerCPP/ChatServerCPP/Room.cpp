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
