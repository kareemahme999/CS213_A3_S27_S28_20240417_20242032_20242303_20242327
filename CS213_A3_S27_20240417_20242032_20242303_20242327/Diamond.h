/**
 * @file Diamond.h
 * @brief Diamond-shaped Tic-Tac-Toe board and UI declarations.
 */
#ifndef DIAMOND_BOARD_H
#define DIAMOND_BOARD_H

#include "BoardGame_Classes.h"
#include <vector>


class DiamondBoard : public Board<char> {
private:
    std::vector<std::vector<bool>> allowed;

public:

    DiamondBoard();

    bool update_board(Move<char>* move) override;

    bool is_win(Player<char>* player) override;

    bool is_lose(Player<char>* player) override;

    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;

private:

    bool inside_diamond(int r, int c) const;

    bool check_line(char sym, int needed) const;
};
// UI Implementation for Diamond Tic-Tac-Toe
class DiamondUI : public UI<char> {
public:

    DiamondUI();
    ~DiamondUI() {};
   virtual Move<char>* get_move(Player<char>* player) override;

    Player<char>* create_player(std::string& name, char symbol, PlayerType t) override;
};

#endif