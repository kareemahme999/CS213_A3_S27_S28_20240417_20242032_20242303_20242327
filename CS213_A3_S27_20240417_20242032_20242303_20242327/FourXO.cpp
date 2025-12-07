#include "FourXO.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor - Start with alternating X O pattern
Board_4x4::Board_4x4() : Board(4, 4), current_player_symbol('X') {
    n_moves = 0;

    // Initialize empty board
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            board[i][j] = ' ';
        }
    }

    // Player X pieces: X in positions (0,0), (0,2), (3,1), (3,3)
    player1_pieces = {{0,0}, {0,2}, {3,1}, {3,3}};

    // Player O pieces: O in positions (0,1), (0,3), (3,0), (3,2)
    player2_pieces = {{0,1}, {0,3}, {3,0}, {3,2}};

    // Place X pieces on board
    for (auto& pos : player1_pieces) {
        board[pos.first][pos.second] = 'X';
    }

    // Place O pieces on board
    for (auto& pos : player2_pieces) {
        board[pos.first][pos.second] = 'O';
    }
}

// Get pieces for a specific player
vector<pair<int, int>> Board_4x4::get_player_pieces(char symbol) {
    if (symbol == 'X') {
        return player1_pieces;
    } else {
        return player2_pieces;
    }
}

// Check if position is valid
bool Board_4x4::isValidPosition(int x, int y) {
    return (x >= 0 && x < 4 && y >= 0 && y < 4);
}

// Move a piece from one position to another
bool Board_4x4::move_piece(int from_x, int from_y, int to_x, int to_y, char symbol) {
    if (!isValidPosition(from_x, from_y) || !isValidPosition(to_x, to_y)) {
        return false;
    }

    // Check if source has player's piece and target is empty
    if (board[from_x][from_y] == symbol && board[to_x][to_y] == ' ') {
        // Check if move is adjacent (up, down, left, right - no diagonal)
        int dx = abs(from_x - to_x);
        int dy = abs(from_y - to_y);

        if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
            // Move the piece
            board[to_x][to_y] = symbol;
            board[from_x][from_y] = ' ';
            n_moves++;

            // Update pieces positions
            if (symbol == 'X') {
                for (auto& pos : player1_pieces) {
                    if (pos.first == from_x && pos.second == from_y) {
                        pos.first = to_x;
                        pos.second = to_y;
                        break;
                    }
                }
            } else {
                for (auto& pos : player2_pieces) {
                    if (pos.first == from_x && pos.second == from_y) {
                        pos.first = to_x;
                        pos.second = to_y;
                        break;
                    }
                }
            }
            return true;
        }
    }

    return false;
}

// Update board with move - handles piece movement
bool Board_4x4::update_board(Move<char>* move) {
    int to_x = move->get_x();
    int to_y = move->get_y();
    char symbol = move->get_symbol();

    // Check if target position is valid and empty
    if (!isValidPosition(to_x, to_y) || board[to_x][to_y] != ' ') {
        return false;
    }

    // Find a piece that can move to the target position
    vector<pair<int, int>> pieces = get_player_pieces(symbol);

    for (auto& piece : pieces) {
        int from_x = piece.first;
        int from_y = piece.second;

        // Check if this piece can move to target position
        int dx = abs(from_x - to_x);
        int dy = abs(from_y - to_y);

        // Valid move: adjacent cell (up, down, left, right)
        if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
            // Move the piece
            board[to_x][to_y] = symbol;
            board[from_x][from_y] = ' ';
            n_moves++;

            // Update pieces positions
            if (symbol == 'X') {
                for (auto& pos : player1_pieces) {
                    if (pos.first == from_x && pos.second == from_y) {
                        pos.first = to_x;
                        pos.second = to_y;
                        break;
                    }
                }
            } else {
                for (auto& pos : player2_pieces) {
                    if (pos.first == from_x && pos.second == from_y) {
                        pos.first = to_x;
                        pos.second = to_y;
                        break;
                    }
                }
            }
            return true;
        }
    }

    return false;
}

// Check for three in a row
bool Board_4x4::checkThreeInRow(char symbol) {
    // Check all possible horizontal lines
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            if (board[i][j] == symbol &&
                board[i][j+1] == symbol &&
                board[i][j+2] == symbol) {
                return true;
            }
        }
    }

    // Check all possible vertical lines
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 2; i++) {
            if (board[i][j] == symbol &&
                board[i+1][j] == symbol &&
                board[i+2][j] == symbol) {
                return true;
            }
        }
    }

    // Check diagonals
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            // Main diagonal
            if (board[i][j] == symbol &&
                board[i+1][j+1] == symbol &&
                board[i+2][j+2] == symbol) {
                return true;
            }
            // Anti-diagonal
            if (board[i][j+2] == symbol &&
                board[i+1][j+1] == symbol &&
                board[i+2][j] == symbol) {
                return true;
            }
        }
    }

    return false;
}

// Check if player has valid moves
bool Board_4x4::has_valid_moves(char symbol) {
    vector<pair<int, int>> pieces = get_player_pieces(symbol);

    for (auto& piece : pieces) {
        int x = piece.first, y = piece.second;

        // Check all four directions
        int directions[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}}; // right, down, left, up

        for (auto& dir : directions) {
            int new_x = x + dir[0];
            int new_y = y + dir[1];

            if (isValidPosition(new_x, new_y) && board[new_x][new_y] == ' ') {
                return true; // Found at least one valid move
            }
        }
    }
    return false; // No valid moves
}

// Check if player has won
bool Board_4x4::is_win(Player<char>* player) {
    return checkThreeInRow(player->get_symbol());
}

// Check if game is draw
bool Board_4x4::is_draw(Player<char>* player) {
    // Game is draw if no valid moves for BOTH players
    return !has_valid_moves('X') && !has_valid_moves('O');
}

// No loser in this game
bool Board_4x4::is_lose(Player<char>* player) {
    return false;
}

// Check if game is over
bool Board_4x4::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// Display the current board state
void Board_4x4::display_board() {
    cout << "\n====== 4x4 MOVING TIC-TAC-TOE ======\n";
    cout << "    0   1   2   3\n";
    cout << "  +---+---+---+---+\n";
    for (int i = 0; i < 4; i++) {
        cout << i << " | ";
        for (int j = 0; j < 4; j++) {
            cout << board[i][j] << " | ";
        }
        cout << "\n  +---+---+---+---+\n";
    }
    cout << "Player X: 4 pieces" << endl;
    cout << "Player O: 4 pieces" << endl;
    cout << "Total moves: " << n_moves << endl;
    cout << "====================================\n";
}

// Get move from player
Move<char>* UI_4x4::get_move(Player<char>* player) {
    // For computer player
    if (player->get_type() == PlayerType::COMPUTER) {
        RandomPlayer_4x4* computer_player = static_cast<RandomPlayer_4x4*>(player);
        return computer_player->get_move();
    }

    // For human player - ask for target position
    int to_x, to_y;
    cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
    cout << "Enter target position to move to (row column): ";
    cin >> to_x >> to_y;

    return new Move<char>(to_x, to_y, player->get_symbol());
}

// Create player based on type
Player<char>* UI_4x4::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::COMPUTER) {
        return new RandomPlayer_4x4(name, symbol);
    }
    return new Player<char>(name, symbol, type);
}

// Computer player makes random moves
Move<char>* RandomPlayer_4x4::get_move() {
    Board_4x4* game_board = static_cast<Board_4x4*>(this->get_board_ptr());

    char symbol = this->get_symbol();
    vector<pair<int, int>> pieces = game_board->get_player_pieces(symbol);
    vector<pair<int, int>> valid_targets;

    // Collect all valid target positions
    for (auto& piece : pieces) {
        int from_x = piece.first;
        int from_y = piece.second;

        int directions[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}}; // right, down, left, up

        for (auto& dir : directions) {
            int to_x = from_x + dir[0];
            int to_y = from_y + dir[1];

            if (game_board->isValidPosition(to_x, to_y)) {
                vector<vector<char>> board_matrix = game_board->get_board_matrix();
                if (board_matrix[to_x][to_y] == ' ') {
                    valid_targets.push_back({to_x, to_y});
                }
            }
        }
    }

    if (!valid_targets.empty()) {
        // Choose random valid target
        int random_index = rand() % valid_targets.size();
        int to_x = valid_targets[random_index].first;
        int to_y = valid_targets[random_index].second;

        cout << "Computer " << this->get_name() << " moves to ("
             << to_x << "," << to_y << ")" << endl;
        return new Move<char>(to_x, to_y, symbol);
    }

    // If no valid moves, try any empty cell
    vector<vector<char>> board_matrix = game_board->get_board_matrix();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board_matrix[i][j] == ' ') {
                cout << "Computer " << this->get_name() << " chooses (" << i << "," << j << ")" << endl;
                return new Move<char>(i, j, symbol);
            }
        }
    }
    // Last resort
    return new Move<char>(0, 0, symbol);
}

