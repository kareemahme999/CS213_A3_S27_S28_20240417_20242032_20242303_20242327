#include "Diamond.h"
#include <iostream>
#include <map>
#include <cstdlib>
#include <ctime>
using namespace std;

DiamondBoard::DiamondBoard() : Board(7, 7)
{
    allowed = {
        {0,0,0,1,0,0,0},
        {0,0,1,1,1,0,0},
        {0,1,1,1,1,1,0},
        {1,1,1,1,1,1,1},
        {0,1,1,1,1,1,0},
        {0,0,1,1,1,0,0},
        {0,0,0,1,0,0,0}
    };

    // Initialize board with '#' for blocked cells
    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 7; ++j)
            board[i][j] = '#';

    // Set allowed cells to empty space
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if (allowed[i][j] == 1)
                board[i][j] = ' ';
        }
    }
}

bool DiamondBoard::inside_diamond(int r, int c) const {
    if (r < 0 || r >= 7 || c < 0 || c >= 7)
        return false;
    return allowed[r][c] == 1;
}

char DiamondBoard::get_cell(int r, int c) const {
    if (r < 0 || r >= 7 || c < 0 || c >= 7)
        return '#';
    return board[r][c];
}

bool DiamondBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Check bounds
    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    // Check if inside diamond
    if (!inside_diamond(x, y))
        return false;

    // Check if cell is empty
    if (board[x][y] != ' ')
        return false;

    board[x][y] = mark;
    n_moves++;
    return true;
}

bool DiamondBoard::check_line(char sym, int need) const {
    // Horizontal
    for (int i = 0; i < 7; ++i) {
        int cnt = 0;
        for (int j = 0; j < 7; ++j) {
            if (board[i][j] == sym) cnt++;
            else cnt = 0;
            if (cnt >= need) return true;
        }
    }

    // Vertical
    for (int j = 0; j < 7; ++j) {
        int cnt = 0;
        for (int i = 0; i < 7; ++i) {
            if (board[i][j] == sym) cnt++;
            else cnt = 0;
            if (cnt >= need) return true;
        }
    }

    // Main diagonal (top-left to bottom-right)
    for (int d = -3; d <= 3; ++d) {
        int cnt = 0;
        for (int i = 0; i < 7; ++i) {
            int j = i + d;
            if (j >= 0 && j < 7 && board[i][j] == sym) cnt++;
            else cnt = 0;
            if (cnt >= need) return true;
        }
    }

    // Anti-diagonal (top-right to bottom-left)
    for (int d = 0; d <= 12; ++d) {
        int cnt = 0;
        for (int i = 0; i < 7; ++i) {
            int j = d - i;
            if (j >= 0 && j < 7 && board[i][j] == sym) cnt++;
            else cnt = 0;
            if (cnt >= need) return true;
        }
    }

    return false;
}

bool DiamondBoard::is_win(Player<char>* player) {
    char sym = player->get_symbol();

    map<string, vector<vector<pair<int, int>>>> lines;
    lines["H"] = {};
    lines["V"] = {};
    lines["MD"] = {};
    lines["AD"] = {};

    auto add_line = [](vector<vector<pair<int, int>>>& container, vector<pair<int, int>>& line) {
        if (!line.empty()) container.push_back(line);
    };

    // Horizontal lines
    for (int i = 0; i < 7; ++i) {
        vector<pair<int, int>> line;
        for (int j = 0; j < 7; ++j) {
            if (board[i][j] == sym) {
                line.push_back({i, j});
            } else {
                if (line.size() >= 3) add_line(lines["H"], line);
                line.clear();
            }
        }
        if (line.size() >= 3) add_line(lines["H"], line);
    }

    // Vertical lines
    for (int j = 0; j < 7; ++j) {
        vector<pair<int, int>> line;
        for (int i = 0; i < 7; ++i) {
            if (board[i][j] == sym) {
                line.push_back({i, j});
            } else {
                if (line.size() >= 3) add_line(lines["V"], line);
                line.clear();
            }
        }
        if (line.size() >= 3) add_line(lines["V"], line);
    }

    // Main diagonal
    for (int d = -3; d <= 3; ++d) {
        vector<pair<int, int>> line;
        for (int i = 0; i < 7; ++i) {
            int j = i + d;
            if (j >= 0 && j < 7 && board[i][j] == sym) {
                line.push_back({i, j});
            } else {
                if (line.size() >= 3) add_line(lines["MD"], line);
                line.clear();
            }
        }
        if (line.size() >= 3) add_line(lines["MD"], line);
    }

    // Anti-diagonal
    for (int d = 0; d <= 12; ++d) {
        vector<pair<int, int>> line;
        for (int i = 0; i < 7; ++i) {
            int j = d - i;
            if (j >= 0 && j < 7 && board[i][j] == sym) {
                line.push_back({i, j});
            } else {
                if (line.size() >= 3) add_line(lines["AD"], line);
                line.clear();
            }
        }
        if (line.size() >= 3) add_line(lines["AD"], line);
    }

    // Check for winning combination: one line of 3 and one line of 4 in different directions
    vector<string> dirs = {"H", "V", "MD", "AD"};

    for (int i = 0; i < dirs.size(); ++i) {
        for (int j = 0; j < dirs.size(); ++j) {
            if (i == j) continue; // Skip same direction

            auto& lines_i = lines[dirs[i]];
            auto& lines_j = lines[dirs[j]];

            for (auto& li : lines_i) {
                if (li.size() < 3) continue;

                for (auto& lj : lines_j) {
                    if (lj.size() < 4) continue;

                    // Count common cells
                    int common = 0;
                    for (auto& pi : li) {
                        for (auto& pj : lj) {
                            if (pi == pj) common++;
                        }
                    }

                    // Win if they share at most 1 cell
                    if (common <= 1) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool DiamondBoard::is_lose(Player<char>* player) {
    return false; // No lose condition in this game
}

bool DiamondBoard::is_draw(Player<char>* player) {
    // Draw if all 25 cells are filled and no one won
    return (n_moves >= 25 && !is_win(player));
}

bool DiamondBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ==================== UI Implementation ====================

DiamondUI::DiamondUI() : UI<char>("Welcome to Diamond Tic-Tac-Toe", 3) {}

Player<char>* DiamondUI::create_player(string& name, char symbol, PlayerType t) {
    return new Player<char>(name, symbol, t);
}

Move<char>* DiamondUI::get_move(Player<char>* player) {
    int r, c;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol()
             << "), enter your move (row column, 0-6): ";
        cin >> r >> c;

        // Handle invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            r = -1;
            c = -1;
        }
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        DiamondBoard* boardPtr = dynamic_cast<DiamondBoard*>(player->get_board_ptr());

        if (!boardPtr) {
            // Fallback to center
            r = 3;
            c = 3;
        } else {
            // Collect all valid empty cells
            vector<pair<int, int>> valid_moves;

            for (int i = 0; i < 7; i++) {
                for (int j = 0; j < 7; j++) {
                    if (boardPtr->inside_diamond(i, j) &&
                        boardPtr->get_cell(i, j) == ' ') {
                        valid_moves.push_back({i, j});
                    }
                }
            }

            // Choose random valid move
            if (!valid_moves.empty()) {
                int idx = rand() % valid_moves.size();
                r = valid_moves[idx].first;
                c = valid_moves[idx].second;

                cout << "Computer (" << player->get_symbol()
                     << ") chose: (" << r << ", " << c << ")\n";
            } else {
                // No valid moves available (shouldn't happen in normal game)
                cout << "Warning: No valid moves found!\n";
                r = 3;
                c = 3;
            }
        }
    }

    return new Move<char>(r, c, player->get_symbol());
}