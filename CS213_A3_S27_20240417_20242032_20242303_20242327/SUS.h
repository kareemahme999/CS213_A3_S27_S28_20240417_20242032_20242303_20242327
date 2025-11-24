/**
 * @file SUS.h
 * @brief Defines the SUS Game specific classes.
 *
 * This file provides:
 * - `SUS_Board`: A specialized board class for the SUS game.
 * - `SUS_UI`: A user interface class tailored to this game's setup and player interaction.
 */

#ifndef SUS_H
#define SUS_H
/*──────▄▌▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
 * ───▄▄██▌█ beep beep--------------
 * ▄▄▄▌▐██▌█ -KAREEM_AHMED------------
 * ███████▌█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌
 * ▀(@)▀▀▀▀(@)(@)▀▀▀▀▀▀▀▀▀▀▀(@)▀-----------------------
 *
*/
#include "BoardGame_Classes.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
using namespace std;

// #############################################################
// 1. SUS_Board Class
// #############################################################

class SUS_Board : public Board<char> {
private:
    char blank_symbol = '.'; // Empty cell symbol
    // Scores now track points for each player based on the final board state.
    // They no longer track 'moves played'.
    int player1_score = 0;   // Player 1 ('S') final score (total SUS sequences)
    int player2_score = 0;   // Player 2 ('U') final score (total SUS sequences)

    // Helper function to count SUS sequences for the **entire board**
    int count_sus_sequences(); // NO 'player_letter' parameter needed

    // Helper to check if three positions form "SUS"
    bool is_sus_sequence(char a, char b, char c);

public:
    SUS_Board();

    bool update_board(Move<char>* move);
    // Added a display function for a proper game loop
    void display_board();

    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; } // is_win handles the loss condition
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);

    // Get scores
    int get_player1_score() { return player1_score; }
    int get_player2_score() { return player2_score; }

    // Update scores after each move (or, more correctly, before determining win/draw)
    void update_scores();
};


// #############################################################
// 2. SUS_UI Class
// ############################################################

class SUS_UI : public UI<char> {
private:
    // Helper to get valid letter choice (S or U)
    char get_valid_letter(Player<char>* player);

public:
    SUS_UI();
    ~SUS_UI() {}

    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
    virtual Player<char>** setup_players();
};

#endif // SUS_H