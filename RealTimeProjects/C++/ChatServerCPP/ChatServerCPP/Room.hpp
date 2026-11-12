//
//  Room.hpp
//  ChatServerCPP
//
//  Created by Anussha on 10/11/26.
//

#ifndef Room_hpp
#define Room_hpp

#pragma once
#include <map>
#include <string>
#include <vector>

struct Room {
    std::string name;
    std::vector<std::string> members;
};

void addMember(Room& room, const std::string& name);
void printMembers(const Room& room);
bool removeMember(Room& room, const std::string& name);
bool roomExists(const std::map<std::string, Room>& rooms, const std::string& name);
bool sameRoom(const std::map<int, std::string>& clientRoom, int a, int b);
bool inRoom(const std::map<int, std::string>& clientRoom, int client);
void joinRoom(std::map<int, std::string>& clientRoom, int client, const std::string& room);
void leaveRoom(std::map<int, std::string>& clientRoom, int client);
#endif /* Room_hpp */
