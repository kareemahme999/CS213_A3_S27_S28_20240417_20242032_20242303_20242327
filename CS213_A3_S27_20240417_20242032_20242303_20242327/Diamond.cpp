
#include "Diamond.h"
#include <iostream>
#include <map>
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

    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 7; ++j)
            board[i][j] = '#';

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if (allowed[i][j] == 1)
                board[i][j] = ' ';
        }
    }
}

bool DiamondBoard::inside_diamond(int r, int c) const {
    return allowed[r][c] == 1;
}


bool DiamondBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    if (!inside_diamond(x, y))
        return false;

    if (board[x][y] != '.')
        return false;

    board[x][y] = mark;
    n_moves++;
    return true;
}

bool DiamondBoard::check_line(char sym, int need) const {
    // H
    for (int i = 0; i < 7; ++i) {
        int cnt = 0;
        for (int j = 0; j < 7; ++j) {
            if (board[i][j] == sym) cnt++;
            else cnt = 0;
            if (cnt >= need) return true;
        }
    }

    // V
    for (int j = 0; j < 7; ++j) {
        int cnt = 0;
        for (int i = 0; i < 7; ++i) {
            if (board[i][j] == sym) cnt++;
            else cnt = 0;
            if (cnt >= need) return true;
        }
    }

	// M_diagonal
    for (int d = -3; d <= 3; ++d) {
        int cnt = 0;
        for (int i = 0; i < 7; ++i) {
            int j = i + d;
            if (j >= 0 && j < 7 && board[i][j] == sym) cnt++;
            else cnt = 0;
            if (cnt >= need) return true;
        }
    }

	// -M_diagonal
    for (int d = 0; d <= 6; ++d) {
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
    lines["MND"] = {};

    auto add_line = [](vector<vector<pair<int, int>>>& container, vector<pair<int, int>>& line) {
        if (!line.empty()) container.push_back(line);
        };

    //  H
    for (int i = 0; i < 7; ++i) {
        vector<pair<int, int>> line;
        for (int j = 0; j < 7; ++j) {
            if (board[i][j] == sym) line.push_back({ i,j });
            else {
                if (line.size() >= 3) add_line(lines["H"], line);
                line.clear();
            }
        }
        if (line.size() >= 3) add_line(lines["H"], line);
    }

    //  V
    for (int j = 0; j < 7; ++j) {
        vector<pair<int, int>> line;
        for (int i = 0; i < 7; ++i) {
            if (board[i][j] == sym) line.push_back({ i,j });
            else {
                if (line.size() >= 3) add_line(lines["V"], line);
                line.clear();
            }
        }
        if (line.size() >= 3) add_line(lines["V"], line);
    }

    // M_diagonal
    for (int d = -3; d <= 3; ++d) {
        vector<pair<int, int>> line;
        for (int i = 0; i < 7; ++i) {
            int j = i + d;
            if (j >= 0 && j < 7 && board[i][j] == sym) line.push_back({ i,j });
            else {
                if (line.size() >= 3) add_line(lines["MD"], line);
                line.clear();
            }
        }
        if (line.size() >= 3) add_line(lines["MD"], line);
    }

    // -M_diagonal
    for (int d = 0; d <= 6; ++d) {
        vector<pair<int, int>> line;
        for (int i = 0; i < 7; ++i) {
            int j = d - i;
            if (j >= 0 && j < 7 && board[i][j] == sym) line.push_back({ i,j });
            else {
                if (line.size() >= 3) add_line(lines["MND"], line);
                line.clear();
            }
        }
        if (line.size() >= 3) add_line(lines["MND"], line);
    }

	// Window checking
    vector<string> dirs = { "H", "V", "MD", "MND" };

    for (int i = 0; i < dirs.size(); ++i) {
        for (int j = 0; j < dirs.size(); ++j) {
			if (i == j) continue; // Skip same direction

            auto& lines3 = lines[dirs[i]];
            auto& lines4 = lines[dirs[j]];

            for (auto& l3 : lines3) {
                if (l3.size() < 3) continue;
                for (auto& l4 : lines4) {
                    if (l4.size() < 4) continue;

                    
                    int common = 0;
                    for (auto& p3 : l3)
                        for (auto& p4 : l4)
                            if (p3 == p4) common++;

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
    return false; 
}


bool DiamondBoard::is_draw(Player<char>* player) {
    return (n_moves == 25);
}


bool DiamondBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}



// UI Implementation

DiamondUI::DiamondUI() : UI<char>("Welcome to Diamond Tic-Tac-Toe", 3) {}

Player<char>* DiamondUI::create_player(string& name, char symbol, PlayerType t) {
    cout << "Creating player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, t);
}

Move<char>* DiamondUI::get_move(Player<char>* player) {
    int r, c;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nEnter your move row and column (0 to 6): ";
        cin >> r >> c;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {

        r = rand() % player->get_board_ptr()->get_rows();
        c = rand() % player->get_board_ptr()->get_columns();
        
      
    }

    return new Move<char>(r, c, player->get_symbol());
}