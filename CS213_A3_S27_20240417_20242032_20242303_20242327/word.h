#ifndef WORD_H
#define WORD_H
#include "BoardGame_Classes.h"
#include <set>
#include <string>
#include <vector>
using namespace std;

// Word Board (3x3)
class WordBoard : public Board<char> {
    set<string> dictionary;

    bool in_bounds(int r, int c) const {
        return r >= 0 && r < 3 && c >= 0 && c < 3;
    }

    bool check_word(const string& w) const {
        string W = w;
        for (auto &c : W) c = toupper(c);
        return dictionary.count(W);
    }

public:
    WordBoard();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override {
        return is_win(player) || is_draw(player);
    }
};

// ===============================
// Word UI
// ===============================
class WordUI : public UI<char> {
public:
    WordUI() : UI("Welcome to Word Tic-Tac-Toe!", 3) {}

    Move<char>* get_move(Player<char>* p) override;

    Player<char>* create_player(string& name, char symbol, PlayerType type) override {
        return new Player<char>(name, symbol, type);
    }
};

#endif
