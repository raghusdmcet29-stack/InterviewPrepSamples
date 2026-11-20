//
//  main.cpp
//  TicTacToeCPP
//
//  Created by Anussha on 19/11/26.
// Entry point of the program. This is the controller: it creates the objects and
// tells them what to do. Game rules (turns, winner) will be added here later.

#include <iostream>
#include "Game.hpp"

int main() {
    Game game;

    game.play(0);  // X top-left
    game.play(3);  // O middle-left
    game.play(4);  // X center
    game.play(5);  // O middle-right
    game.play(8);  // X bottom-right, completes the diagonal

    std::cout << "status: " << game.status() << "\n";
    game.print();
    bool accepted = game.play(1);  // try an extra move after X has won
    std::cout << "extra move accepted: " << accepted << "\n";
    game.print();
    return 0;
}
