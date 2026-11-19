//
//  Game.cpp
//  TicTacToeCPP
//
//  Created by Anussha on 19/11/26.
//  Defines the Game functions.

#include "Game.hpp"

Game::Game() : turn_('X') {  // the constructor starts with X
}

char Game::currentPlayer() const {
    return turn_;
}

// A move is accepted only if the cell is legal. Then the turn switches.
bool Game::play(int cell) {
    if (!board_.canPlay(cell)) {    // illegal move: do nothing
        return false;
    }
    board_.set(cell, turn_);        // place the current player's mark
    turn_ = (turn_ == 'X') ? 'O' : 'X';  // switch to the other player
    return true;
}

void Game::print() const {
    board_.print();
}
