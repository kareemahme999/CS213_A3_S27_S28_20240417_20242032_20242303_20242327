#ifndef FOURXO_H
#define FOURXO_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

using namespace std;

// Board class for 4x4 Moving Tic-Tac-Toe
// Players move pieces instead of placing marks
class Board_4x4 : public Board<char> {
private:
    vector<pair<int, int>> player1_pieces; // X player pieces positions
    vector<pair<int, int>> player2_pieces; // O player pieces positions
    char current_player_symbol;

public:
    Board_4x4();
    bool update_board(Move<char>* move) override;
    void display_board();
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;

private:
    bool isValidMove(int from_x, int from_y, int to_x, int to_y, char symbol);
    void initializePieces();
    bool checkThreeInRow(char symbol);
};

// UI class for 4x4 game - handles move input
class UI_4x4 : public UI<char> {
public:
    UI_4x4() : UI("=== 4x4 Moving Tic-Tac-Toe ===", 3) {}
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

// Main function to run the 4x4 game
inline void run_4x4_game();

#endif