#ifndef MISERE_TIC_TAC_TEO_H
#define MISERE_TIC_TAC_TEO_H
#include "BoardGame_Classes.h"
using namespace std;

class Misere_Deep_AI_Player;

class MiserBoard : public Board<char> {
private:
    char blank_symbol = '.';

public:
    MiserBoard();

    MiserBoard(const MiserBoard& other);

    bool update_board(Move<char>* move);

    bool is_win(Player<char>* player);

    bool is_lose(Player<char>*player);

    bool is_draw(Player<char>* player);

    bool game_is_over(Player<char>* player);

    char get_blank_symbol() const { return blank_symbol; }
};

class MiserUI : public UI<char> {
public:
    MiserUI();

    ~MiserUI() {};

    PlayerType get_player_type_choice(string player_label, const vector<string>& options) override;

    Player<char>* create_player(string& name, char symbol, PlayerType type);

    virtual Move<char>* get_move(Player<char>* player);

    Player<char>** setup_players() override;
};

class Misere_Deep_AI_Player : public Player<char> {
private:
    const int MAX_DEPTH = 9;

    int evaluate(MiserBoard* board);
    int minimax(MiserBoard* board, int depth, int alpha, int beta, bool is_maximizer, char current_symbol);

    char get_opponent_symbol() const {
        return (symbol == 'X') ? 'O' : 'X';
    }

public:
    Misere_Deep_AI_Player(string name, char symbol) : Player(name, symbol, PlayerType::AI) {}

    Move<char>* get_move();
};

#endif