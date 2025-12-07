#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include "XO_Classes.h"

/*──────▄▌▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
 * ───▄▄██▌█ beep beep--------------
 * ▄▄▄▌▐██▌█ -KAREEM_AHMED------------
 * ███████▌█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌
 * ▀(@)▀▀▀▀(@)(@)▀▀▀▀▀▀▀▀▀▀▀(@)▀-----------------------
 *
*/
using namespace std;

//--------------------------------------- X_O_Board Implementation

X_O_Board::X_O_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

X_O_Board::X_O_Board(const X_O_Board& other) : Board<char>(other.rows, other.columns) {
    this->board = other.board;
    this->n_moves = other.n_moves;
}

bool X_O_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) {
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

bool X_O_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

bool X_O_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//------------------------------------- XO_UI Implementation

XO_UI::XO_UI() : UI<char>("", 3) {}

PlayerType XO_UI::get_player_type_choice(string player_label, const vector<string>& options) {
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

Player<char>** XO_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer (Random)", "AI (Deep Search)" };

    cout << "\n=== X-O Game Setup ===\n\n";

    cout << "--- Player X ---\n";
    string nameX = get_player_name("Player X");
    PlayerType typeX = get_player_type_choice("Player X", type_options);

    cout << "\n--- Player O ---\n";
    string nameO = get_player_name("Player O");
    PlayerType typeO = get_player_type_choice("Player O", type_options);

    players[0] = create_player(nameX, 'X', typeX);
    players[1] = create_player(nameO, 'O', typeO);

    return players;
}

Player<char>* XO_UI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::AI) {
        cout << "Deep AI Player: " << name << " (Symbol: " << symbol << ")\n";
        return new XO_Deep_AI_Player(name, symbol);
    }

    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* XO_UI::get_move(Player<char>* player) {
    int x, y;

    XO_Deep_AI_Player* ai_player = dynamic_cast<XO_Deep_AI_Player*>(player);
    if (ai_player) {
        if (ai_player->get_board_ptr() != nullptr) {
            return ai_player->get_move();
        } else {
            cout << "Error: AI player board not set!\n";
            return new Move<char>(0, 0, player->get_symbol());
        }
    }
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}

int XO_Deep_AI_Player::evaluate(X_O_Board* board) {
    Player<char> temp_me("temp", symbol, PlayerType::AI);
    Player<char> temp_opp("temp", get_opponent_symbol(), PlayerType::AI);

    if (board->is_win(&temp_me)) {
        return 10;
    }
    if (board->is_win(&temp_opp)) {
        return -10;
    }

    return 0;
}

int XO_Deep_AI_Player::minimax(X_O_Board* board, int depth, int alpha, int beta, 
                                bool is_maximizer, char current_symbol) {
    Player<char> temp_player("temp", current_symbol, PlayerType::AI);

    if (board->game_is_over(&temp_player)) {
        return evaluate(board);
    }

    if (is_maximizer) {
        int max_eval = -10000;

        for (int i = 0; i < board->get_rows(); i++) {
            for (int j = 0; j < board->get_columns(); j++) {
                if (board->get_cell(i, j) == board->get_blank_symbol()) {
                    X_O_Board temp_board = *board;
                    Move<char> move(i, j, current_symbol);
                    temp_board.update_board(&move);

                    int eval = minimax(&temp_board, depth - 1, alpha, beta, false, get_opponent_symbol());
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
        int min_eval = 10000;

        for (int i = 0; i < board->get_rows(); i++) {
            for (int j = 0; j < board->get_columns(); j++) {
                if (board->get_cell(i, j) == board->get_blank_symbol()) {
                    X_O_Board temp_board = *board;
                    Move<char> move(i, j, current_symbol);
                    temp_board.update_board(&move);

                    int eval = minimax(&temp_board, depth - 1, alpha, beta, true, symbol);
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

Move<char>* XO_Deep_AI_Player::get_move() {
    if (!boardPtr) {
        cout << "Error: Board pointer not set for AI player!\n";
        return new Move<char>(0, 0, symbol);
    }

    X_O_Board* xo_board = dynamic_cast<X_O_Board*>(boardPtr);
    if (!xo_board) {
        cout << "Error: Invalid board type for AI player!\n";
        return new Move<char>(0, 0, symbol);
    }

    int best_val = -10000;
    int best_x = -1, best_y = -1;

    cout << "\nDeep AI " << get_name() << " is thinking...\n";

    for (int i = 0; i < xo_board->get_rows(); i++) {
        for (int j = 0; j < xo_board->get_columns(); j++) {
            if (xo_board->get_cell(i, j) == xo_board->get_blank_symbol()) {
                X_O_Board temp_board = *xo_board;
                Move<char> move(i, j, symbol);
                temp_board.update_board(&move);

                int move_val = minimax(&temp_board, MAX_DEPTH - 1, -10000, 10000, false, get_opponent_symbol());

                if (move_val > best_val) {
                    best_val = move_val;
                    best_x = i;
                    best_y = j;
                }
            }
        }
    }

    if (best_x == -1 || best_y == -1) {
        for (int i = 0; i < xo_board->get_rows(); i++) {
            for (int j = 0; j < xo_board->get_columns(); j++) {
                if (xo_board->get_cell(i, j) == xo_board->get_blank_symbol()) {
                    best_x = i;
                    best_y = j;
                    break;
                }
            }
            if (best_x != -1) break;
        }
    }

    cout << "Deep AI " << get_name() << " placed '" << symbol
         << "' at (" << best_x << ", " << best_y << ")";
    cout << " [Evaluation: " << best_val << "]\n";

    return new Move<char>(best_x, best_y, symbol);
}