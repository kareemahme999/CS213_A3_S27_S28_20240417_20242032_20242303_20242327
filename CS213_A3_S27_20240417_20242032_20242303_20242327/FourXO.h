/**
 * @file FourXO.h
 * @brief 4x4 moving Tic-Tac-Toe (pieces move) declarations.
 */
#ifndef FOURXO_H
#define FOURXO_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

using namespace std;

class Board_4x4 : public Board<char> {
private:
    vector<pair<int, int>> player1_pieces; // X player pieces
    vector<pair<int, int>> player2_pieces; // O player pieces
    char current_player_symbol;

public:
    Board_4x4();
    bool update_board(Move<char>* move) override;
    void display_board();
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;

    // Helper functions
    vector<pair<int, int>> get_player_pieces(char symbol);
    bool isValidPosition(int x, int y);
    bool move_piece(int from_x, int from_y, int to_x, int to_y, char symbol);
    bool has_valid_moves(char symbol);

private:
    bool checkThreeInRow(char symbol);
};

class UI_4x4 : public UI<char> {
public:
    UI_4x4() : UI("=== 4x4 Moving Tic-Tac-Toe ===", 3) {}
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

class RandomPlayer_4x4 : public Player<char> {
public:
    RandomPlayer_4x4(string name, char symbol)
        : Player(name, symbol, PlayerType::COMPUTER) {}

    Move<char>* get_move();
};

void run_4x4_game();

#endif