//
//  Board.cpp
//  TicTacToeCPP
//
//  Created by Anussha on 19/11/26.
// Defines the Board functions declared in Board.h.

#include "Board.hpp"  // bring in the declarations
#include <iostream>   // gives us std::cout for printing

// Constructor: called automatically when a Board is created
Board::Board() {
    cells_.fill(' ');   // set all 9 cells to a space (empty)
}

// Prints the grid in the 3x3 shape
void Board::print() const {
    std::cout << " " << cells_[0] << " | " << cells_[1] << " | " << cells_[2] << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << cells_[3] << " | " << cells_[4] << " | " << cells_[5] << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << cells_[6] << " | " << cells_[7] << " | " << cells_[8] << "\n";
}

// Puts a mark into one cell. The controller decides which cell and which mark.
void Board::set(int cell, char mark) {
    cells_[cell] = mark;
}

// Returns true only if the move is legal: cell in range and not already taken
bool Board::canPlay(int cell) const {
    if (cell < 0 || cell > 8) {     // outside the board
        return false;
    }
    return cells_[cell] == ' ';     // true only if the cell is empty
}

// Checks the 8 winning lines. Returns the winning mark, or ' ' if there is no winner yet.
char Board::winner() const {
    // each row holds the 3 cell numbers of one winning line
    const int lines[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},   // rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},   // columns
        {0, 4, 8}, {2, 4, 6}               // diagonals
    };

    for (int i = 0; i < 8; i++) {
        int a = lines[i][0];
        int b = lines[i][1];
        int c = lines[i][2];

        // all three must match, and the first must not be empty
        // (otherwise three empty cells would count as a win)
        if (cells_[a] != ' ' && cells_[a] == cells_[b] && cells_[a] == cells_[c]) {
            return cells_[a];   // this mark is the winner
        }
    }
    return ' ';   // no line is complete
}

// Returns true only if no cell is empty
bool Board::isFull() const {
    for (int i = 0; i < 9; i++) {      // check each of the 9 cells
        if (cells_[i] == ' ') {        // found an empty cell
            return false;              // the board is not full
        }
    }
    return true;                       // no empty cell was found
}
