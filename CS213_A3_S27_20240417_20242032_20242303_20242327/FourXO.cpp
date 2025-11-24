#include "FourXO.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor - initialize 4x4 board with starting pieces
Board_4x4::Board_4x4() : Board(4, 4) {
    n_moves = 0;
    current_player_symbol = 'X';

    // Initialize empty board
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            board[i][j] = ' ';
        }
    }

    initializePieces();
}

// Set up initial piece positions
void Board_4x4::initializePieces() {
    // Player X pieces on first row - store positions only
    player1_pieces = {{0,0}, {0,1}, {0,2}, {0,3}};
    // Player O pieces on last row - store positions only
    player2_pieces = {{3,0}, {3,1}, {3,2}, {3,3}};

    // Keep board empty at start - players will place marks during game
    for (auto& pos : player1_pieces)
        board[pos.first][pos.second] = ' ';  // Empty
    for (auto& pos : player2_pieces)
        board[pos.first][pos.second] = ' ';  // Empty
}

// Check if move is valid
bool Board_4x4::isValidMove(int from_x, int from_y, int to_x, int to_y, char symbol) {
    // Check boundaries
    if (from_x < 0 || from_x >= 4 || from_y < 0 || from_y >= 4) return false;
    if (to_x < 0 || to_x >= 4 || to_y < 0 || to_y >= 4) return false;

    // Check if target cell is empty
    if (board[to_x][to_y] != ' ') return false;

    // Check if piece belongs to current player
    if (board[from_x][from_y] != symbol) return false;

    // Check if move is to adjacent cell (horizontal or vertical only)
    int dx = abs(from_x - to_x);
    int dy = abs(from_y - to_y);
    return (dx == 1 && dy == 0) || (dx == 0 && dy == 1);
}

// Update board with move - needs enhancement for piece movement
bool Board_4x4::update_board(Move<char>* move) {
    // Basic implementation - needs to handle piece movement logic
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (x >= 0 && x < 4 && y >= 0 && y < 4 && board[x][y] == ' ') {
        board[x][y] = symbol;
        n_moves++;
        return true;
    }
    return false;
}

// Check for three in a row
bool Board_4x4::checkThreeInRow(char symbol) {
    // Check horizontal lines
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            if (board[i][j] == symbol && board[i][j+1] == symbol && board[i][j+2] == symbol)
                return true;
        }
    }

    // Check vertical lines
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 2; i++) {
            if (board[i][j] == symbol && board[i+1][j] == symbol && board[i+2][j] == symbol)
                return true;
        }
    }

    // Check diagonals
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            // Main diagonal
            if (board[i][j] == symbol && board[i+1][j+1] == symbol && board[i+2][j+2] == symbol)
                return true;
            // Anti-diagonal
            if (board[i][j+2] == symbol && board[i+1][j+1] == symbol && board[i+2][j] == symbol)
                return true;
        }
    }

    return false;
}

// Check if player has won
bool Board_4x4::is_win(Player<char>* player) {
    return checkThreeInRow(player->get_symbol());
}

// Check if game is draw
bool Board_4x4::is_draw(Player<char>* player) {
    return n_moves >= 16 && !is_win(player); // Board full, no winner
}

// No direct loser in this game
bool Board_4x4::is_lose(Player<char>* player) {
    return false;
}

// Check if game is over
bool Board_4x4::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// Display the board
void Board_4x4::display_board() {
    cout << "\n    0   1   2   3\n";
    cout << "  +---+---+---+---+\n";
    for (int i = 0; i < 4; i++) {
        cout << i << " | ";
        for (int j = 0; j < 4; j++) {
            cout << board[i][j] << " | ";
        }
        cout << "\n  +---+---+---+---+\n";
    }
}

// Get move from player
Move<char>* UI_4x4::get_move(Player<char>* player) {
    int x, y;

    cout << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
    cout << "Enter position (row column): ";
    cin >> x >> y;

    return new Move<char>(x, y, player->get_symbol());
}

// Create player based on type
Player<char>* UI_4x4::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

// MAIN FUNCTION - Run the complete 4x4 game
void run_4x4_game() {
    cout << "\n=====================================\n";
    cout << "    Starting 4x4 Moving Tic-Tac-Toe  \n";
    cout << "=====================================\n";

    // Create game components
    Board_4x4* board = new Board_4x4();
    UI_4x4* ui = new UI_4x4();

    // Setup players
    Player<char>** players = ui->setup_players();

    // Create and run game manager
    GameManager<char> game_manager(board, players, ui);
    game_manager.run();

    // Cleanup memory
    delete board;
    delete ui;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;

    cout << "\n--- 4x4 Game Over ---\n";
}