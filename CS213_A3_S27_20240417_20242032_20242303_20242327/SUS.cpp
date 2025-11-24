#include "SUS.h"
/*──────▄▌▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
 * ───▄▄██▌█ beep beep--------------
 * ▄▄▄▌▐██▌█ -KAREEM_AHMED------------
 * ███████▌█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌
 * ▀(@)▀▀▀▀(@)(@)▀▀▀▀▀▀▀▀▀▀▀(@)▀-----------------------
 *
*/
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <iomanip>
#include <limits>
#include <vector>
using namespace std;

// #############################################################
// SUS_Board Implementation
// #############################################################

SUS_Board::SUS_Board() : Board(3, 3) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            board[i][j] = blank_symbol;
}

void SUS_Board::display_board() {
    cout << "\n    0   1   2\n";
    cout << "  -------------\n";
    for (int i = 0; i < rows; ++i) {
        cout << i << " | ";
        for (int j = 0; j < columns; ++j) {
            cout << board[i][j] << " | ";
        }
        cout << "\n  -------------\n";
    }
    // عرض النقاط باستخدام Player 1 و Player 2
    cout << "Current Score: Player 1(S) = " << player1_score << ", Player 2(U) = " << player2_score << "\n";
}

bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char letter = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol)) {

        // Store previous SUS count BEFORE placing the piece
        int prev_sus_count = count_sus_sequences();

        // Place the piece
        board[x][y] = letter;
        n_moves++;

        // Count NEW SUS sequences created by THIS move
        int new_sus_count = count_sus_sequences();
        int sequences_created = new_sus_count - prev_sus_count;

        // Award points to the CURRENT player based on whose turn it is
        if (n_moves % 2 == 1) {
            // Player 1's turn
            player1_score += sequences_created;
        } else {
            // Player 2's turn
            player2_score += sequences_created;
        }

        return true;
    }

    return false;
}

bool SUS_Board::is_sus_sequence(char a, char b, char c) {
    return (a == 'S' && b == 'U' && c == 'S');
}

int SUS_Board::count_sus_sequences() {
    int count = 0;

    // Check all rows
    for (int i = 0; i < rows; ++i) {
        if (is_sus_sequence(board[i][0], board[i][1], board[i][2]))
            count++;
    }

    // Check all columns
    for (int j = 0; j < columns; ++j) {
        if (is_sus_sequence(board[0][j], board[1][j], board[2][j]))
            count++;
    }

    // Check positive diagonal (top-left to bottom-right)
    if (is_sus_sequence(board[0][0], board[1][1], board[2][2]))
        count++;

    // Check negative diagonal (top-right to bottom-left)
    if (is_sus_sequence(board[0][2], board[1][1], board[2][0]))
        count++;

    return count;
}

void SUS_Board::update_scores() {
    // Scores are already updated during update_board()
}

bool SUS_Board::is_win(Player<char>* player) {
    if (n_moves == 9) {
        // Game is over, check who has more points
        if (player->get_symbol() == 'S') {
            return player1_score > player2_score;
        }
        else if (player->get_symbol() == 'U') {
            return player2_score > player1_score;
        }
    }
    return false;
}

bool SUS_Board::is_draw(Player<char>* player) {
    if (n_moves == 9) {
        // Draw ONLY if scores are exactly equal
        return (player1_score == player2_score);
    }
    return false;
}

bool SUS_Board::game_is_over(Player<char>* player) {
    return (n_moves == 9);
}

// #############################################################
// SUS_UI Implementation
// #############################################################

SUS_UI::SUS_UI() : UI<char>("", 3) {
}

Player<char>** SUS_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    cout << "\n=== SUS Game Setup ===\n";
    cout << "Rules: Create 'S-U-S' sequences to score points!\n";
    cout << "Player 1 must place 'S', Player 2 must place 'U' on their turn.\n\n";

    cout << "--- Player 1 (Will play FIRST) ---\n";
    string name1 = get_player_name("Player 1");
    PlayerType type1 = get_player_type_choice("Player 1", type_options);

    cout << "\n--- Player 2 (Will play SECOND) ---\n";
    string name2 = get_player_name("Player 2");
    PlayerType type2 = get_player_type_choice("Player 2", type_options);

    players[0] = create_player(name1, 'S', type1);
    players[1] = create_player(name2, 'U', type2);

    return players;
}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    // تبسيط رسالة الإنشاء
    cout << (type == PlayerType::HUMAN ? "Human" : "Computer")
         << " Player: " << name << " (Symbol: " << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

// Helper function to evaluate a potential move (used by Computer)
int evaluate_move(SUS_Board* sus_board, int x, int y, char letter) {
    // Create a temporary copy of the board state
    char temp_board[3][3];
    auto board_matrix = sus_board->get_board_matrix();

    // Copy current board state
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp_board[i][j] = board_matrix[i][j];
        }
    }

    // Count current SUS sequences
    int current_sus = 0;

    // Check all existing SUS sequences BEFORE the move (Rows, Cols, Diagonals)
    for (int i = 0; i < 3; i++) {
        if (temp_board[i][0] == 'S' && temp_board[i][1] == 'U' && temp_board[i][2] == 'S')
            current_sus++;
    }
    for (int j = 0; j < 3; j++) {
        if (temp_board[0][j] == 'S' && temp_board[1][j] == 'U' && temp_board[2][j] == 'S')
            current_sus++;
    }
    if (temp_board[0][0] == 'S' && temp_board[1][1] == 'U' && temp_board[2][2] == 'S')
        current_sus++;
    if (temp_board[0][2] == 'S' && temp_board[1][1] == 'U' && temp_board[2][0] == 'S')
        current_sus++;

    // Now place the new piece
    temp_board[x][y] = letter;

    // Count NEW SUS sequences after the move
    int new_sus = 0;

    // Check all new SUS sequences (Rows, Cols, Diagonals)
    for (int i = 0; i < 3; i++) {
        if (temp_board[i][0] == 'S' && temp_board[i][1] == 'U' && temp_board[i][2] == 'S')
            new_sus++;
    }
    for (int j = 0; j < 3; j++) {
        if (temp_board[0][j] == 'S' && temp_board[1][j] == 'U' && temp_board[2][j] == 'S')
            new_sus++;
    }
    if (temp_board[0][0] == 'S' && temp_board[1][1] == 'U' && temp_board[2][2] == 'S')
        new_sus++;
    if (temp_board[0][2] == 'S' && temp_board[1][1] == 'U' && temp_board[2][0] == 'S')
        new_sus++;

    // Return the number of NEW sequences this move creates
    return new_sus - current_sus;
}

Move<char>* SUS_UI::get_move(Player<char>* player) {
    int x, y;
    char chosen_letter = player->get_symbol();
    Board<char>* board_ptr = player->get_board_ptr();

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << "'s turn.\n";

        // رسالة الحرف الثابت
        cout << "Your letter for this turn is: '" << chosen_letter << "'.\n";

        // Get coordinates
        bool valid_coords = false;
        while (!valid_coords) {
            cout << "Enter coordinates (x y) to place '" << chosen_letter << "' (0 to 2):\n";
            cout << "Enter x (row): ";
            if (!(cin >> x)) {
                cout << "Invalid input. Please enter a number for x.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cout << "Enter y (column): ";
            if (!(cin >> y)) {
                cout << "Invalid input. Please enter a number for y.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (x >= 0 && x < board_ptr->get_rows() && y >= 0 && y < board_ptr->get_columns()) {
                valid_coords = true;
            } else {
                cout << "Invalid coordinates. x and y must be between 0 and 2.\n";
            }
        }

    } else if (player->get_type() == PlayerType::COMPUTER) {
        char blank_sym = '.';
        auto board_matrix = board_ptr->get_board_matrix();

        // Cast board_ptr to SUS_Board to use evaluate_move
        SUS_Board* sus_board = dynamic_cast<SUS_Board*>(board_ptr);

        // Store all possible moves with their scores
        struct MoveOption {
            int x, y, score;
            char letter;
        };
        vector<MoveOption> all_moves;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board_matrix[i][j] == blank_sym) {
                    int score = evaluate_move(sus_board, i, j, chosen_letter);
                    all_moves.push_back({i, j, score, chosen_letter});
                }
            }
        }

        // Find the best score
        int best_score = -1;
        for (const auto& move : all_moves) {
            if (move.score > best_score) {
                best_score = move.score;
            }
        }

        // Collect all moves with the best score
        vector<MoveOption> best_moves;
        for (const auto& move : all_moves) {
            if (move.score == best_score) {
                best_moves.push_back(move);
            }
        }

        // Randomly choose from the best moves
        MoveOption chosen = best_moves[rand() % best_moves.size()];

        x = chosen.x;
        y = chosen.y;

        cout << "\nComputer " << player->get_name()
             << " placed '" << chosen_letter << "' at ("
             << x << ", " << y << ")";
        if (best_score > 0) {
            cout << " [Scored " << best_score;
        }
        cout << "\n";
    }

    return new Move<char>(x, y, chosen_letter);
}