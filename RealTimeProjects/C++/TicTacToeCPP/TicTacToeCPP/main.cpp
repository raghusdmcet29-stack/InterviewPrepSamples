//
//  main.cpp
//  TicTacToeCPP
//
//  Created by Anussha on 19/11/26.
// Entry point of the program. This is the controller: it creates the objects and
// tells them what to do. Game rules (turns, winner) will be added here later.

#include <iostream>
#include <map>
#include <string>
#include "httplib.h"
#include "Game.hpp"

// One game room: the game itself plus the token for each player.
struct Room {
    Game game;
    std::string xToken;
    std::string oToken;
};
// All rooms, keyed by room number. Lives in memory, so it is empty after a restart.
std::map<int, Room> rooms;

// The number the next created room will get.
int nextRoomId = 1;


int main() {
    // The HTTP server. Routes are registered on it before listen() starts it.
    httplib::Server server;
    
    // Simple check that the server is up.
    server.Get("/hello", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("hello", "text/plain");
    });

    server.set_default_headers({
        {"Access-Control-Allow-Origin", "*"}
    });
    // Echoes back whatever text is in the URL.
    server.Get("/echo/:text", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content(req.path_params.at("text"), "text/plain");
    });

    // Creates a room and gives the creator the X token.
    server.Post("/rooms", [](const httplib::Request&, httplib::Response& res) {
        int id = nextRoomId++;
        Room room;
        room.xToken = "x" + std::to_string(id);
        room.oToken = "o" + std::to_string(id);
        rooms[id] = room;

        res.set_content("{\"roomId\":" + std::to_string(id) +
                        ",\"token\":\"" + room.xToken + "\",\"symbol\":\"X\"}",
                        "application/json");
    });
    
    // Second player joins an existing room and gets the O token.
    server.Post(R"(/rooms/(\d+)/join)", [](const httplib::Request& req, httplib::Response& res) {
        // The room number from the URL, e.g. 1 in /rooms/1/join.
        int id = std::stoi(req.matches[1]);
        auto it = rooms.find(id);
        if (it == rooms.end()) {
            res.status = 404;
            res.set_content("{\"error\":\"room not found\"}", "application/json");
            return;
        }

        res.set_content("{\"roomId\":" + std::to_string(id) +
                        ",\"token\":\"" + it->second.oToken + "\",\"symbol\":\"O\"}",
                        "application/json");
    });
   
    // Applies a move. Checks the room, then the token, then the game rules.
    server.Post(R"(/rooms/(\d+)/move)", [](const httplib::Request& req, httplib::Response& res) {
        // The room number from the URL, e.g. 1 in /rooms/1/move.
        int id = std::stoi(req.matches[1]);
        auto it = rooms.find(id);
        if (it == rooms.end()) {
            res.status = 404;
            res.set_content("{\"error\":\"room not found\"}", "application/json");
            return;
        }
        
        // Values from the query string, e.g. ?token=x1&cell=0.
        std::string token = req.get_param_value("token");
        int cell = std::stoi(req.get_param_value("cell"));

        // Work out which player the token belongs to.
        char symbol;
        if (token == it->second.xToken) symbol = 'X';
        else if (token == it->second.oToken) symbol = 'O';
        else {
            res.status = 403;
            res.set_content("{\"error\":\"invalid token\"}", "application/json");
            return;
        }
        
        // New check: refuse the move if it is not this player's turn.
        if (symbol != it->second.game.currentPlayer()) {
            res.status = 409;
            res.set_content("{\"error\":\"not your turn\"}", "application/json");
            return;
        }

        // The game decides whether the move is legal.
        bool accepted = it->second.game.play(cell);
        res.set_content("{\"accepted\":" + std::string(accepted ? "true" : "false") +
                        ",\"player\":\"" + std::string(1, symbol) + "\"}",
                        "application/json");
    });
    
    server.Get(R"(/rooms/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        // The room number from the URL, e.g. 1 in /rooms/1.
        int id = std::stoi(req.matches[1]);
        auto it = rooms.find(id);
        if (it == rooms.end()) {
            res.status = 404;
            res.set_content("{\"error\":\"room not found\"}", "application/json");
            return;
        }

        // Build the board as a string of 9 characters, using '.' for empty cells.
        std::string board;
        for (int i = 0; i < 9; i++) {
            char c = it->second.game.cellAt(i);
            board += (c == ' ') ? '.' : c;
        }

        res.set_content("{\"board\":\"" + board +
                        "\",\"turn\":\"" + std::string(1, it->second.game.currentPlayer()) +
                        "\",\"status\":\"" + it->second.game.status() + "\"}",
                        "application/json");
    });

    // Start the server on localhost, port 8080. This call keeps running until the program stops.
    std::cout << "server running on port 8080\n";
    server.listen("127.0.0.1", 8080);
    return 0;
}
