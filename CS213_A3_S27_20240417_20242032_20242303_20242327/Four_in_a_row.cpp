#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>
#include <cstdlib>
#include "Four_in_a_row.h"
using namespace std;

/*──────▄▌▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
 * ───▄▄██▌█ beep beep--------------
 * ▄▄▄▌▐██▌█ -KAREEM_AHMED------------
 * ███████▌█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌
 * ▀(@)▀▀▀▀(@)(@)▀▀▀▀▀▀▀▀▀▀▀(@)▀-----------------------
 *
*/

// Helper function للتحقق من 4 متتالية
bool check_four_in_a_row(const std::vector<std::vector<char>>& board, int r, int c,
                         int dr, int dc, char symbol) {
    int rows = board.size();
    int cols = board[0].size();
    int count = 0;

    for (int i = 0; i < 4; ++i) {
        int nr = r + i * dr;
        int nc = c + i * dc;

        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && board[nr][nc] == symbol) {
            count++;
        } else {
            return false;
        }
    }
    return count == 4;
}

// #############################################################
// FourInARow_Board Implementation
// #############################################################

FourInARow_Board::FourInARow_Board() : Board<char>(6, 7) {
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = blank_symbol;
        }
    }
}

FourInARow_Board::FourInARow_Board(const FourInARow_Board& other)
    : Board<char>(other.rows, other.columns) {
    this->board = other.board;
    this->n_moves = other.n_moves;
}

int FourInARow_Board::get_available_row(int y) const {
    for (int i = rows - 1; i >= 0; --i) {
        if (board[i][y] == blank_symbol) {
            return i;
        }
    }
    return -1;
}

bool FourInARow_Board::update_board(Move<char>* move) {
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (y < 0 || y >= columns) {
        return false;
    }

    int x = get_available_row(y);

    if (x != -1) {
        n_moves++;
        board[x][y] = symbol;
        return true;
    }
    return false;
}

bool FourInARow_Board::is_win(Player<char>* player) {
    char symbol = player->get_symbol();

    int dr[] = {0, 1, 1, 1};
    int dc[] = {1, 0, 1, -1};

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (board[r][c] == symbol) {
                for (int i = 0; i < 4; ++i) {
                    if (check_four_in_a_row(board, r, c, dr[i], dc[i], symbol)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool FourInARow_Board::is_draw(Player<char>* player) {
    return (n_moves == rows * columns && !is_win(player));
}

bool FourInARow_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// #############################################################
// FourInARow_UI Implementation
// #############################################################

FourInARow_UI::FourInARow_UI() : UI<char>("", 6) {
}

PlayerType FourInARow_UI::get_player_type_choice(string player_label,
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

Player<char>** FourInARow_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];

    vector<string> type_options = { "Human", "Computer (Random)", "AI (Deep Search)" };

    cout << "\n=== Four-in-a-Row Game Setup ===\n";
    cout << "Rules: Connect 4 pieces in a row (horizontally, vertically, or diagonally) to win!\n\n";

    cout << "--- Player 1 (X) ---\n";
    string name1 = get_player_name("Player 1");
    PlayerType type1 = get_player_type_choice("Player 1", type_options);

    cout << "\n--- Player 2 (O) ---\n";
    string name2 = get_player_name("Player 2");
    PlayerType type2 = get_player_type_choice("Player 2", type_options);

    players[0] = create_player(name1, 'X', type1);
    players[1] = create_player(name2, 'O', type2);

    return players;
}

Player<char>* FourInARow_UI::create_player(string& name, char symbol, PlayerType type) {
    //  FourInARow_Deep_AI_Playez
    if (type == PlayerType::AI) {
        cout << "Deep AI Player: " << name << " (Symbol: " << symbol << ")\n";
        return new FourInARow_Deep_AI_Player(name, symbol);
    }

    cout << (type == PlayerType::HUMAN ? "Human" : "Computer (Random)")
         << " Player: " << name << " (Symbol: " << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

int FourInARow_UI::get_valid_column(Player<char>* player) {
    int column;
    int cols = player->get_board_ptr()->get_columns();
    FourInARow_Board* board_ptr = dynamic_cast<FourInARow_Board*>(player->get_board_ptr());

    while (true) {
        cout << "Enter column number (0 to " << cols - 1 << "): ";
        if (!(cin >> column)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (column >= 0 && column < cols) {
            if (board_ptr->get_available_row(column) != -1) {
                return column;
            } else {
                cout << "Column " << column << " is full. Try another column.\n";
            }
        } else {
            cout << "Invalid column number. Must be between 0 and " << cols - 1 << ".\n";
        }
    }
}

Move<char>* FourInARow_UI::get_move(Player<char>* player) {
    int y;
    char symbol = player->get_symbol();
    int cols = player->get_board_ptr()->get_columns();


    FourInARow_Deep_AI_Player* ai_player = dynamic_cast<FourInARow_Deep_AI_Player*>(player);
    if (ai_player) {
        if (ai_player->get_board_ptr() != nullptr) {
            return ai_player->get_move();
        } else {
            cout << "Error: AI player board not set!\n";
            return new Move<char>(-1, 0, symbol);
        }
    }

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlayer " << player->get_name() << " (" << symbol << ")'s turn.\n";
        y = get_valid_column(player);

    } else if (player->get_type() == PlayerType::COMPUTER) {
        FourInARow_Board* board_ptr = dynamic_cast<FourInARow_Board*>(player->get_board_ptr());

        do {
            y = rand() % cols;
        } while (board_ptr->get_available_row(y) == -1);

        cout << "\nComputer player " << player->get_name()
             << " (" << symbol << ") chose column " << y << "\n";
    }

    return new Move<char>(-1, y, symbol);
}

// #############################################################
//  FourInARow_Deep_AI_Player Implementation - كلها جديدة!
// #############################################################

int FourInARow_Deep_AI_Player::count_sequences(FourInARow_Board* board,
                                                char symbol, int length) {
    int count = 0;
    auto board_matrix = board->get_board_matrix();
    int rows = board->get_rows();
    int cols = board->get_columns();
    char blank = board->get_blank_symbol();

    int dr[4] = {0, 1, 1, 1};
    int dc[4] = {1, 0, 1, -1};

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            for (int dir = 0; dir < 4; ++dir) {
                int seq_count = 0;
                int empty_count = 0;

                for (int i = 0; i < 4; ++i) {
                    int nr = r + i * dr[dir];
                    int nc = c + i * dc[dir];

                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) {
                        seq_count = 0;
                        break;
                    }

                    if (board_matrix[nr][nc] == symbol) {
                        seq_count++;
                    } else if (board_matrix[nr][nc] == blank) {
                        empty_count++;
                    } else {
                        seq_count = 0;
                        break;
                    }
                }

                if (seq_count == length && empty_count > 0) {
                    count++;
                }
            }
        }
    }

    return count;
}

int FourInARow_Deep_AI_Player::evaluate(FourInARow_Board* board) {
    // إنشاء player مؤقت للفحص
    Player<char> temp_me("temp", symbol, PlayerType::AI);
    Player<char> temp_opp("temp", get_opponent_symbol(), PlayerType::AI);

    // فحص الفوز/الخسارة
    if (board->is_win(&temp_me)) {
        return 10000; // الفوز = أفضل شيء
    }
    if (board->is_win(&temp_opp)) {
        return -10000; // الخسارة = أسوأ شيء
    }

    int score = 0;

    // تقييم بناءً على المتتاليات
    // 3 متتالية = قوية جداً (قريبة من الفوز!)
    score += count_sequences(board, symbol, 3) * 100;
    score -= count_sequences(board, get_opponent_symbol(), 3) * 100;

    // 2 متتالية = جيدة
    score += count_sequences(board, symbol, 2) * 10;
    score -= count_sequences(board, get_opponent_symbol(), 2) * 10;


    auto board_matrix = board->get_board_matrix();
    for (int i = 0; i < board->get_rows(); ++i) {
        for (int j = 0; j < board->get_columns(); ++j) {
            if (board_matrix[i][j] == symbol) {
                // الأعمدة الوسطى (3) أفضل
                score += (3 - abs(j - 3));
            }
        }
    }

    return score;
}


int FourInARow_Deep_AI_Player::minimax(FourInARow_Board* board, int depth,
                                       int alpha, int beta,
                                       bool is_maximizer, char current_symbol) {
    Player<char> temp_player("temp", current_symbol, PlayerType::AI);

    // Base cases
    if (depth == 0 || board->game_is_over(&temp_player)) {
        return evaluate(board);
    }

    if (is_maximizer) {
        int max_eval = -100000;

        // جرب كل عمود ممكن
        for (int col = 0; col < board->get_columns(); col++) {
            if (board->get_available_row(col) != -1) {
                // عمل نسخة من اللوحة
                FourInARow_Board temp_board = *board;
                Move<char> move(-1, col, current_symbol);
                temp_board.update_board(&move);

                // استدعاء recursive للخصم
                int eval = minimax(&temp_board, depth - 1, alpha, beta, false,
                                  get_opponent_symbol());
                max_eval = std::max(max_eval, eval);

                alpha = std::max(alpha, max_eval);
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return max_eval;

    } else {
        int min_eval = 100000;

        for (int col = 0; col < board->get_columns(); col++) {
            if (board->get_available_row(col) != -1) {
                FourInARow_Board temp_board = *board;
                Move<char> move(-1, col, current_symbol);
                temp_board.update_board(&move);

                int eval = minimax(&temp_board, depth - 1, alpha, beta, true, symbol);
                min_eval = std::min(min_eval, eval);

                beta = std::min(beta, min_eval);
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return min_eval;
    }
}


Move<char>* FourInARow_Deep_AI_Player::get_move() {
    if (!boardPtr) {
        cout << "Error: Board pointer not set for AI player!\n";
        return new Move<char>(-1, 0, symbol);
    }

    FourInARow_Board* four_board = dynamic_cast<FourInARow_Board*>(boardPtr);
    if (!four_board) {
        cout << "Error: Invalid board type for AI player!\n";
        return new Move<char>(-1, 0, symbol);
    }

    int best_val = -100000;
    int best_col = -1;

    cout << "\n🤖 Deep AI " << get_name() << " is thinking...\n";

    // جرب كل عمود واختار الأفضل
    for (int col = 0; col < four_board->get_columns(); col++) {
        if (four_board->get_available_row(col) != -1) {
            FourInARow_Board temp_board = *four_board;
            Move<char> move(-1, col, symbol);
            temp_board.update_board(&move);

            // تقييم الحركة باستخدام minimax
            int move_val = minimax(&temp_board, MAX_DEPTH - 1, -100000, 100000,
                                  false, get_opponent_symbol());

            if (move_val > best_val) {
                best_val = move_val;
                best_col = col;
            }
        }
    }

    // Fallback إذا لم نجد عمود (نادر جداً)
    if (best_col == -1) {
        for (int col = 0; col < four_board->get_columns(); col++) {
            if (four_board->get_available_row(col) != -1) {
                best_col = col;
                break;
            }
        }
    }

    cout << " Deep AI " << get_name() << " placed '" << symbol
         << "' in column " << best_col;
    cout << " [Evaluation: " << best_val << "]\n";

    return new Move<char>(-1, best_col, symbol);
}