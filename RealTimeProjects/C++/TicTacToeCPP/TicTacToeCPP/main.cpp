//
//  main.cpp
//  TicTacToeCPP
//
//  Created by Anussha on 19/11/26.
// Entry point of the program. This is the controller: it creates the objects and
// tells them what to do. Game rules (turns, winner) will be added here later.

#include <iostream>
#include "Board.hpp"

int main() {
    Board board;
    std::cout << "empty: " << board.isFull() << "\n";

    for (int cell = 0; cell < 9; cell++) {
        board.set(cell, 'X');
    }
    std::cout << "full: " << board.isFull() << "\n";
    return 0;
}
