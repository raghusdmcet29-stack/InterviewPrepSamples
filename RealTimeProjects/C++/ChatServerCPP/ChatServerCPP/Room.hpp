//
//  Room.hpp
//  ChatServerCPP
//
//  Created by Anussha on 10/11/26.
//

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

bool sameRoom(const std::map<int, std::string>& clientRoom, int a, int b);
bool inRoom(const std::map<int, std::string>& clientRoom, int client);
void joinRoom(std::map<int, std::string>& clientRoom, int client, const std::string& room);
void leaveRoom(std::map<int, std::string>& clientRoom, int client);

#endif /* Room_hpp */
