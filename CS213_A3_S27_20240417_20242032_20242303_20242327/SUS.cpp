#include "SUS.h"
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <iomanip>
#include <limits>
#include <algorithm>
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

SUS_Board::SUS_Board(const SUS_Board& other)
    : Board<char>(other.rows, other.columns) {
    this->board = other.board;
    this->n_moves = other.n_moves;
    this->player1_score = other.player1_score;
    this->player2_score = other.player2_score;
    this->blank_symbol = other.blank_symbol;
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
    cout << "Current Score: Player 1(S) = " << player1_score
         << ", Player 2(U) = " << player2_score << "\n";
}

bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char letter = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol)) {

        int prev_sus_count = count_sus_sequences();
        board[x][y] = letter;
        n_moves++;

        int new_sus_count = count_sus_sequences();
        int sequences_created = new_sus_count - prev_sus_count;

        if (n_moves % 2 == 1) {
            player1_score += sequences_created;
        } else {
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

    for (int i = 0; i < rows; ++i) {
        if (is_sus_sequence(board[i][0], board[i][1], board[i][2]))
            count++;
    }

    for (int j = 0; j < columns; ++j) {
        if (is_sus_sequence(board[0][j], board[1][j], board[2][j]))
            count++;
    }

    if (is_sus_sequence(board[0][0], board[1][1], board[2][2]))
        count++;

    if (is_sus_sequence(board[0][2], board[1][1], board[2][0]))
        count++;

    return count;
}

void SUS_Board::update_scores() {
}

bool SUS_Board::is_win(Player<char>* player) {
    if (n_moves == 9) {
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

PlayerType SUS_UI::get_player_type_choice(string player_label,
                                          const vector<string>& options) {
    cout << "Choose " << player_label << " type:\n";
    for (size_t i = 0; i < options.size(); ++i)
        cout << i + 1 << ". " << options[i] << "\n";

    int choice;
    cout << "Please Choose type of player: ";
    cin >> choice;

    if (choice == 1) return PlayerType::HUMAN;
    if (choice == 2) return PlayerType::COMPUTER;
    if (choice == 3) return PlayerType::AI;

    cout << "Invalid choice! Defaulting to Human.\n";
    return PlayerType::HUMAN;
}

Player<char>** SUS_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer (Greedy)", "AI (Deep Search)" };

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
    if (type == PlayerType::AI) {
        cout << "Deep AI Player: " << name << " (Symbol: " << symbol << ")\n";
        return new SUS_Deep_AI_Player(name, symbol);
    }

    cout << (type == PlayerType::HUMAN ? "Human" : "Computer (Greedy)")
         << " Player: " << name << " (Symbol: " << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

int evaluate_move(SUS_Board* sus_board, int x, int y, char letter) {
    char temp_board[3][3];
    auto board_matrix = sus_board->get_board_matrix();

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp_board[i][j] = board_matrix[i][j];
        }
    }

    int current_sus = sus_board->count_sus_sequences();
    temp_board[x][y] = letter;

    int new_sus = 0;

    for (int i = 0; i < 3; ++i) {
        if (temp_board[i][0] == 'S' && temp_board[i][1] == 'U' && temp_board[i][2] == 'S')
            new_sus++;
    }
    for (int j = 0; j < 3; ++j) {
        if (temp_board[0][j] == 'S' && temp_board[1][j] == 'U' && temp_board[2][j] == 'S')
            new_sus++;
    }
    if (temp_board[0][0] == 'S' && temp_board[1][1] == 'U' && temp_board[2][2] == 'S')
        new_sus++;
    if (temp_board[0][2] == 'S' && temp_board[1][1] == 'U' && temp_board[2][0] == 'S')
        new_sus++;

    return new_sus - current_sus;
}


Move<char>* SUS_UI::get_move(Player<char>* player) {
    int x, y;
    char chosen_letter = player->get_symbol();
    Board<char>* board_ptr = player->get_board_ptr();

    SUS_Deep_AI_Player* ai_player = dynamic_cast<SUS_Deep_AI_Player*>(player);
    if (ai_player) {
        if (ai_player->get_board_ptr() != nullptr) {
            return ai_player->get_move();
        } else {
            cout << "Error: AI player board not set!\n";
            return new Move<char>(0, 0, chosen_letter);
        }
    }

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << "'s turn.\n";
        cout << "Your letter for this turn is: '" << chosen_letter << "'.\n";

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

        return new Move<char>(x, y, chosen_letter);

    } else {
        // Computer (Greedy) player
        char blank_sym = '.';
        auto board_matrix = board_ptr->get_board_matrix();
        SUS_Board* sus_board = dynamic_cast<SUS_Board*>(board_ptr);

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

        int best_score = -1;
        for (const auto& move : all_moves) {
            if (move.score > best_score) {
                best_score = move.score;
            }
        }

        vector<MoveOption> best_moves;
        for (const auto& move : all_moves) {
            if (move.score == best_score) {
                best_moves.push_back(move);
            }
        }

        MoveOption chosen = best_moves[rand() % best_moves.size()];
        x = chosen.x;
        y = chosen.y;

        cout << "\nComputer " << player->get_name()
             << " placed '" << chosen_letter << "' at ("
             << x << ", " << y << ")";
        if (best_score > 0) {
            cout << " [Scored " << best_score << "]";
        }
        cout << "\n";

        return new Move<char>(x, y, chosen_letter);
    }
}

// #############################################################
// SUS_Deep_AI_Player Implementation
// #############################################################

int SUS_Deep_AI_Player::evaluate(SUS_Board* board) {
    if (get_symbol() == 'S') {
        return board->get_player1_score() - board->get_player2_score();
    } else {
        return board->get_player2_score() - board->get_player1_score();
    }
}

int SUS_Deep_AI_Player::minimax(SUS_Board* board, int depth, int alpha, int beta, bool is_maximizer) {
    if (depth == 0 || board->game_is_over(this)) {
        return evaluate(board);
    }

    char current_player_symbol = (board->get_n_moves() % 2 == 0) ? 'S' : 'U';

    if (is_maximizer) {
        int max_eval = -100000;

        for (int i = 0; i < board->get_rows(); i++) {
            for (int j = 0; j < board->get_columns(); j++) {
                if (board->get_cell(i, j) == '.') {
                    SUS_Board temp_board = *board;
                    Move<char> current_move(i, j, current_player_symbol);
                    temp_board.update_board(&current_move);

                    int eval = minimax(&temp_board, depth - 1, alpha, beta, false);
                    max_eval = std::max(max_eval, eval);

                    alpha = std::max(alpha, max_eval);
                    if (beta <= alpha) {
                        return max_eval;
                    }
                }
            }
        }
        return max_eval;

    } else {
        int min_eval = 100000;

        for (int i = 0; i < board->get_rows(); i++) {
            for (int j = 0; j < board->get_columns(); j++) {
                if (board->get_cell(i, j) == '.') {
                    SUS_Board temp_board = *board;
                    Move<char> current_move(i, j, current_player_symbol);
                    temp_board.update_board(&current_move);

                    int eval = minimax(&temp_board, depth - 1, alpha, beta, true);
                    min_eval = std::min(min_eval, eval);

                    beta = std::min(beta, min_eval);
                    if (beta <= alpha) {
                        return min_eval;
                    }
                }
            }
        }
        return min_eval;
    }
}

Move<char>* SUS_Deep_AI_Player::get_move() {
    if (!boardPtr) {
        cout << "Error: Board pointer not set for AI player!\n";
        return new Move<char>(0, 0, symbol);
    }

    SUS_Board* sus_board = dynamic_cast<SUS_Board*>(boardPtr);
    if (!sus_board) {
        cout << "Error: Invalid board type for AI player!\n";
        return new Move<char>(0, 0, symbol);
    }

    int best_val = -100000;
    int best_x = -1, best_y = -1;

    for (int i = 0; i < sus_board->get_rows(); i++) {
        for (int j = 0; j < sus_board->get_columns(); j++) {
            if (sus_board->get_cell(i, j) == '.') {
                SUS_Board temp_board = *sus_board;
                Move<char> current_move(i, j, symbol);
                temp_board.update_board(&current_move);

                int move_val = minimax(&temp_board, MAX_DEPTH - 1, -100000, 100000, false);

                if (move_val > best_val) {
                    best_val = move_val;
                    best_x = i;
                    best_y = j;
                }
            }
        }
    }

    if (best_x == -1 || best_y == -1) {
        for (int i = 0; i < sus_board->get_rows(); i++) {
            for (int j = 0; j < sus_board->get_columns(); j++) {
                if (sus_board->get_cell(i, j) == '.') {
                    best_x = i;
                    best_y = j;
                    break;
                }
            }
            if (best_x != -1) break;
        }
    }

    cout << "\nDeep AI " << get_name() << " placed '" << symbol
         << "' at (" << best_x << ", " << best_y << ")";
    cout << " [Evaluation: " << best_val << "]\n";

    return new Move<char>(best_x, best_y, symbol);
}