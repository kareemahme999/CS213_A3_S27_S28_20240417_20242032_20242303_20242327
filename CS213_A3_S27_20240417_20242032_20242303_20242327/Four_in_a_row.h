/**
 * @file Four_in_a_row.h
 * @brief Four-in-a-row (Connect-like) board and UI declarations.
 */

#ifndef FOUR_IN_A_ROW_H
#define FOUR_IN_A_ROW_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>
using namespace std;

/*──────▄▌▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
 * ───▄▄██▌█ beep beep--------------
 * ▄▄▄▌▐██▌█ -KAREEM_AHMED------------
 * ███████▌█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌
 * ▀(@)▀▀▀▀(@)(@)▀▀▀▀▀▀▀▀▀▀▀(@)▀-----------------------
 *
*/

class FourInARow_Deep_AI_Player;

class FourInARow_Board : public Board<char> {
private:
    const char blank_symbol = ' ';

public:
    FourInARow_Board();
    
    FourInARow_Board(const FourInARow_Board& other);

    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; }
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
    
    int get_available_row(int y) const;
    
    char get_blank_symbol() const { return blank_symbol; }
};

// #############################################################
// 2. FourInARow_UI Class
// #############################################################

class FourInARow_UI : public UI<char> {
private:
    int get_valid_column(Player<char>* player);

public:
    FourInARow_UI();
    ~FourInARow_UI() {}

    PlayerType get_player_type_choice(string player_label,
                                      const vector<string>& options) override;

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
    Player<char>** setup_players() override;
};

// #############################################################
// 3. FourInARow_Deep_AI_Player Class - جديدة بالكامل
// #############################################################

class FourInARow_Deep_AI_Player : public Player<char> {
private:
    const int MAX_DEPTH = 6; // عمق البحث (يمكن تقليله لـ 4-5 للسرعة)

    int evaluate(FourInARow_Board* board);
    
    int count_sequences(FourInARow_Board* board, char symbol, int length);
    
    int minimax(FourInARow_Board* board, int depth, int alpha, int beta,
                bool is_maximizer, char current_symbol);
    
    char get_opponent_symbol() const {
        return (symbol == 'X') ? 'O' : 'X';
    }

public:
    FourInARow_Deep_AI_Player(string name, char symbol)
        : Player(name, symbol, PlayerType::AI) {}

    Move<char>* get_move();
};
#endif // FOUR_IN_A_ROW_H