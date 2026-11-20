//
//  Game.hpp
//  TicTacToeCPP
//
//  Created by Anussha on 19/11/26.
// Declares the Game class: the rules controller. It owns the board and tracks whose turn it is.

#ifndef Game_hpp
#define Game_hpp
#include "Board.hpp"    // Game uses a Board
#include <string>

class Game {
public:
    Game();                  // starts a new game, X goes first
    char currentPlayer() const;  // returns 'X' or 'O'
    bool play(int cell);     // tries a move for the current player; true if it was accepted
    void print() const;      // shows the board
    char cellAt(int index) const;  // reads one cell through the board
    std::string status() const;
    
private:
    Board board_;            // the grid
    char turn_;              // whose turn it is: 'X' or 'O'
};


#endif /* Game_hpp */
