//
//  Board.hpp
//  TicTacToeCPP
//
//  Created by Anussha on 19/11/26.
//

// Declares the Board class: the 9 cells of the game and the functions that work on them.
// This is the "model" part. It stores data and knows nothing about turns or players./

#ifndef Board_hpp
#define Board_hpp

#include <array>        // gives us std::array, a fixed-size list

class Board{
public:
    Board();  // constructor: runs when a Board is created, sets up an empty board
    void print() const; // prints the grid to the screen; "const" means it does not change the board
    void set(int cell, char mark);  // puts 'X' or 'O' into the given cell (0 to 8)
    bool canPlay(int cell) const;   // true if cell is 0 to 8 and empty
    char cell(int index) const;  // returns ' ', 'X', or 'O' for the given cell (0 to 8)
    char winner() const;   // returns 'X' or 'O' if someone has a line, otherwise ' '
    bool isFull() const;   // true if all 9 cells are filled
    
private:
    std::array<char, 9> cells_; // the 9 cells, each ' ', 'X', or 'O'; private so only Board can touch them

};

#endif /* Board_hpp */
