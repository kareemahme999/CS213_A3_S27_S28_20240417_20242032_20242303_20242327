/**
 * @file SUS.h
 * @brief Declarations for the SUS game (S-U-S sequence scoring) and its UI/AI.
 */
#ifndef SUS_H
#define SUS_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <cctype>
using namespace std;

// #############################################################
// 1. SUS_Board Class
// #############################################################

class SUS_Board : public Board<char> {
private:
    char blank_symbol = '.';
    int player1_score = 0;
    int player2_score = 0;

    bool is_sus_sequence(char a, char b, char c);

public:
    int count_sus_sequences();

    SUS_Board();

    SUS_Board(const SUS_Board& other);

    bool update_board(Move<char>* move);
    void display_board();

    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; }
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);

    int get_player1_score() { return player1_score; }
    int get_player2_score() { return player2_score; }

    void update_scores();
};

// #############################################################
// 2. SUS_UI Class
// #############################################################

class SUS_UI : public UI<char> {
public:
    SUS_UI();
    ~SUS_UI() {}

    // ✅ Override للدالة المسؤولة عن اختيار نوع اللاعب
    PlayerType get_player_type_choice(string player_label,
                                      const vector<string>& options) override;

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
    Player<char>** setup_players() override;
};

// #############################################################
// 3. SUS_Deep_AI_Player Class
// #############################################################

class SUS_Deep_AI_Player : public Player<char> {
private:
    const int MAX_DEPTH = 9;

    int evaluate(SUS_Board* board);
    int minimax(SUS_Board* board, int depth, int alpha, int beta, bool is_maximizer);

public:
    SUS_Deep_AI_Player(string name, char symbol)
        : Player(name, symbol, PlayerType::AI) {}

    Move<char>* get_move();
};

#endif // SUS_H