#include <iostream>
#include <iomanip>
#include <cctype>  // for toupper()
#include <algorithm>
#include "BoardGame_Classes.h"
#include "Misere_Tic_Tac_Teo.h"
using namespace std;

//--------------------------------------- Misere Implementation
MiserBoard::MiserBoard() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

MiserBoard::MiserBoard(const MiserBoard& other) : Board<char>(other.rows, other.columns) {
    this->board = other.board;
    this->n_moves = other.n_moves;
}

bool MiserBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();//x,o

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {         // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

bool MiserBoard::is_win(Player<char>* player) {
    return false;
}

bool MiserBoard::is_lose(Player<char>* player) {
    const char sym = player->get_symbol();
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && c != blank_symbol;
    };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]))&&board[i][0]==sym) return true;
        if ((all_equal(board[0][i], board[1][i], board[2][i]))&&board[0][i]==sym) return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]))&&board[0][0]==sym) return true;
    if ((all_equal(board[0][2], board[1][1], board[2][0]))&&board[1][1]==sym) return true;

    return false;
}
bool MiserBoard::is_draw(Player<char>* player) {
    return (n_moves == 9 &&!is_lose(player));
}

bool MiserBoard::game_is_over(Player<char>* player) {
    return is_lose(player) || is_draw(player);
}

//------------------------------------- MiserUI Implementation

MiserUI::MiserUI() : UI<char>("", 3) {}

PlayerType MiserUI::get_player_type_choice(string player_label, const vector<string>& options) {
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

Player<char>** MiserUI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer (Random)", "AI (Deep Search)" };

    cout << "\n=== Misere Tic-Tac-Toe Game Setup ===\n";
    cout << "Rules: Avoid getting three-in-a-row! The player who makes three-in-a-row LOSES!\n\n";

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

Player<char>* MiserUI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::AI) {
        cout << "Deep AI Player: " << name << " (Symbol: " << symbol << ")\n";
        return new Misere_Deep_AI_Player(name, symbol);
    }

    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* MiserUI::get_move(Player<char>* player) {
    int x, y;

    Misere_Deep_AI_Player* ai_player = dynamic_cast<Misere_Deep_AI_Player*>(player);
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

int Misere_Deep_AI_Player::evaluate(MiserBoard* board) {
    Player<char> temp_me("temp", symbol, PlayerType::AI);
    Player<char> temp_opp("temp", get_opponent_symbol(), PlayerType::AI);

    if (board->is_lose(&temp_me)) {
        return -10;
    }
    if (board->is_lose(&temp_opp)) {
        return 10;
    }

    return 0;
}

int Misere_Deep_AI_Player::minimax(MiserBoard* board, int depth, int alpha, int beta,
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
                    MiserBoard temp_board = *board;
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
                    MiserBoard temp_board = *board;
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

Move<char>* Misere_Deep_AI_Player::get_move() {
    if (!boardPtr) {
        cout << "Error: Board pointer not set for AI player!\n";
        return new Move<char>(0, 0, symbol);
    }

    MiserBoard* miser_board = dynamic_cast<MiserBoard*>(boardPtr);
    if (!miser_board) {
        cout << "Error: Invalid board type for AI player!\n";
        return new Move<char>(0, 0, symbol);
    }

    int best_val = -10000;
    int best_x = -1, best_y = -1;

    cout << "\nDeep AI " << get_name() << " is thinking...\n";

    for (int i = 0; i < miser_board->get_rows(); i++) {
        for (int j = 0; j < miser_board->get_columns(); j++) {
            if (miser_board->get_cell(i, j) == miser_board->get_blank_symbol()) {
                MiserBoard temp_board = *miser_board;
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
        for (int i = 0; i < miser_board->get_rows(); i++) {
            for (int j = 0; j < miser_board->get_columns(); j++) {
                if (miser_board->get_cell(i, j) == miser_board->get_blank_symbol()) {
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