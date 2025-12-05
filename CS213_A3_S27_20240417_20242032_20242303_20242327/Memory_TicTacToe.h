#ifndef MEMORY_TICTACTOE_H
#define MEMORY_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>

using namespace std;

class Memory_TicTacToe : public Board<char> {
private:
    vector<vector<char>> board;  // 3x3 grid
    int n_moves;

public:
    Memory_TicTacToe();

    // Override virtual functions
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Display board (marks hidden)
    void display_board();

    // Accessors
    char get_cell(int r, int c);
};

class MemoryUI : public UI<char> {
public:
    MemoryUI();
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

#endif
