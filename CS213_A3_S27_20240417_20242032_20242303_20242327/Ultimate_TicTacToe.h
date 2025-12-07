#ifndef ULTIMATE_TICTACTOE_H
#define ULTIMATE_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

class Ultimate_TicTacToe : public Board<char> {
private:
    vector<vector<char>> big_board;    // المربع 3x3 (reference board)
    vector<vector<char>> board;        // المربع 9x9 للعب
    int n_moves;

public:
    Ultimate_TicTacToe();

    // Override Board virtual functions
    bool update_board(Move<char>* move) override;
    void display_board() ;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Sub-board operations (للمربع 9x9)
    void display_subboard();
    bool subboard_is_full();
    void reset_subboard();

    // Big board operations (للمربع 3x3)
    void display_big_board();
    char get_bigboard_cell(int r, int c);
    void set_bigboard_cell(int r, int c, char sym);

    // Sub-board access (للمربع 9x9)
    vector<vector<char>> get_subboard();
    char get_subboard_cell(int r, int c);
    void set_subboard_cell(int r, int c, char sym);

    // Helper functions
    bool check_small_grid_win(int start_r, int start_c, char sym);
};

// -------------------- UI --------------------
class UltimateUI : public UI<char> {
public:
    UltimateUI();
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    pair<int,int> choose_bigboard_cell(Player<char>* player, Ultimate_TicTacToe* board);
};

#endif