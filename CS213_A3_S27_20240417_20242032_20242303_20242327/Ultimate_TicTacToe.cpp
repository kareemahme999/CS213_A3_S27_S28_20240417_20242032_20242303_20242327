#include "Ultimate_TicTacToe.h"

// -------------------- Constructor --------------------
Ultimate_TicTacToe::Ultimate_TicTacToe() : Board<char>(9, 9), n_moves(0) {
    // المربع الكبير 9x9 (للعب)
    board = vector<vector<char>>(9, vector<char>(9, '-'));

    // المربع الصغير 3x3 (خلف الكواليس - reference board)
    big_board = vector<vector<char>>(3, vector<char>(3, '-'));
}

// -------------------- Update Board --------------------
bool Ultimate_TicTacToe::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char sym = move->get_symbol();

    if(r < 0 || r > 8 || c < 0 || c > 8) return false;
    if(board[r][c] != '-') return false;

    board[r][c] = sym;
    n_moves++;
    return true;
}

// -------------------- Display Board --------------------
void Ultimate_TicTacToe::display_board() {
    cout << "\n          0      1      2      3      4      5      6      7      8\n";
    cout << "   -----------------------------------------------------------------------\n";

    for(int i = 0; i < 9; i++) {
        cout << " " << i << " |";
        for(int j = 0; j < 9; j++) {
            cout << "   " << board[i][j] << "   |";
        }
        cout << "\n";
        cout << "   -----------------------------------------------------------------------\n";
    }
}

// -------------------- Sub-board (9x9) --------------------
void Ultimate_TicTacToe::display_subboard() {
    cout << "\n          0      1      2      3      4      5      6      7      8\n";
    cout << "   -----------------------------------------------------------------------\n";

    for(int i = 0; i < 9; i++) {
        cout << " " << i << " |";
        for(int j = 0; j < 9; j++) {
            cout << "   " << board[i][j] << "   |";
        }
        cout << "\n";
        cout << "   -----------------------------------------------------------------------\n";
    }
}

bool Ultimate_TicTacToe::subboard_is_full() {
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            if(board[i][j] == '-') return false;
    return true;
}

void Ultimate_TicTacToe::reset_subboard() {
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            board[i][j] = '-';
    n_moves = 0;
}

// -------------------- Big board (3x3) --------------------
void Ultimate_TicTacToe::display_big_board() {
    // مش بنعرض حاجة - ال big_board للحسابات الداخلية بس
}

char Ultimate_TicTacToe::get_bigboard_cell(int r, int c) {
    return big_board[r][c];
}

void Ultimate_TicTacToe::set_bigboard_cell(int r, int c, char sym) {
    big_board[r][c] = sym;
}

// -------------------- Sub-board getters/setters --------------------
vector<vector<char>> Ultimate_TicTacToe::get_subboard() {
    return board;
}

char Ultimate_TicTacToe::get_subboard_cell(int r, int c) {
    return board[r][c];
}

void Ultimate_TicTacToe::set_subboard_cell(int r, int c, char sym) {
    board[r][c] = sym;
}

// -------------------- Check Small Grid Win --------------------
bool Ultimate_TicTacToe::check_small_grid_win(int start_r, int start_c, char sym) {
    // فحص الصفوف في المربع الصغير 3x3
    for(int i = 0; i < 3; i++) {
        if(board[start_r + i][start_c] == sym &&
           board[start_r + i][start_c + 1] == sym &&
           board[start_r + i][start_c + 2] == sym) {
            return true;
        }
    }

    // فحص الأعمدة
    for(int j = 0; j < 3; j++) {
        if(board[start_r][start_c + j] == sym &&
           board[start_r + 1][start_c + j] == sym &&
           board[start_r + 2][start_c + j] == sym) {
            return true;
        }
    }

    // فحص القطر الأول
    if(board[start_r][start_c] == sym &&
       board[start_r + 1][start_c + 1] == sym &&
       board[start_r + 2][start_c + 2] == sym) {
        return true;
    }

    // فحص القطر الثاني
    if(board[start_r][start_c + 2] == sym &&
       board[start_r + 1][start_c + 1] == sym &&
       board[start_r + 2][start_c] == sym) {
        return true;
    }

    return false;
}

// -------------------- Win/Lose/Draw (للمربع 3x3) --------------------
bool Ultimate_TicTacToe::is_win(Player<char>* player) {
    char sym = player->get_symbol();

    // فحص الصفوف في big_board
    for(int i = 0; i < 3; i++) {
        if(big_board[i][0] == sym &&
           big_board[i][1] == sym &&
           big_board[i][2] == sym) return true;
    }

    // فحص الأعمدة
    for(int j = 0; j < 3; j++) {
        if(big_board[0][j] == sym &&
           big_board[1][j] == sym &&
           big_board[2][j] == sym) return true;
    }

    // فحص الأقطار
    if(big_board[0][0] == sym &&
       big_board[1][1] == sym &&
       big_board[2][2] == sym) return true;

    if(big_board[0][2] == sym &&
       big_board[1][1] == sym &&
       big_board[2][0] == sym) return true;

    return false;
}

bool Ultimate_TicTacToe::is_lose(Player<char>* player) {
    char other = (player->get_symbol() == 'X') ? 'O' : 'X';

    // فحص الصفوف
    for(int i = 0; i < 3; i++) {
        if(big_board[i][0] == other &&
           big_board[i][1] == other &&
           big_board[i][2] == other) return true;
    }

    // فحص الأعمدة
    for(int j = 0; j < 3; j++) {
        if(big_board[0][j] == other &&
           big_board[1][j] == other &&
           big_board[2][j] == other) return true;
    }

    // فحص الأقطار
    if(big_board[0][0] == other &&
       big_board[1][1] == other &&
       big_board[2][2] == other) return true;

    if(big_board[0][2] == other &&
       big_board[1][1] == other &&
       big_board[2][0] == other) return true;

    return false;
}

bool Ultimate_TicTacToe::is_draw(Player<char>* player) {
    // لو big_board كله اتملى ومفيش حد كسب
    bool full = true;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(big_board[i][j] == '-') {
                full = false;
                break;
            }
        }
        if(!full) break;
    }

    return full && !is_win(player) && !is_lose(player);
}

bool Ultimate_TicTacToe::game_is_over(Player<char>* player) {
    return is_win(player) || is_lose(player) || is_draw(player);
}

// -------------------- UI --------------------
UltimateUI::UltimateUI() : UI("Ultimate Tic Tac Toe", 2) {
    srand(time(0));
}

Move<char>* UltimateUI::get_move(Player<char>* player) {
    Ultimate_TicTacToe* b = dynamic_cast<Ultimate_TicTacToe*>(player->get_board_ptr());

    if(player->get_type() == PlayerType::COMPUTER) {
        int r, c;
        do {
            r = rand() % 9;
            c = rand() % 9;
        } while(b->get_subboard_cell(r, c) != '-');

        cout << "Computer plays: " << r << " " << c << "\n";
        return new Move<char>(r, c, player->get_symbol());
    }
    else {
        int r, c;
        cout << player->get_name() << " (" << player->get_symbol()
             << ") enter row col (0-8): ";
        cin >> r >> c;
        return new Move<char>(r, c, player->get_symbol());
    }
}

Player<char>* UltimateUI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

pair<int,int> UltimateUI::choose_bigboard_cell(Player<char>* player, Ultimate_TicTacToe* board) {
    int r, c;
    while(true) {
        if(player->get_type() == PlayerType::COMPUTER) {
            r = rand() % 3;
            c = rand() % 3;
        }
        else {
            cout << player->get_name() << " choose big-board cell row col (0-2): ";
            cin >> r >> c;
        }

        if(r < 0 || r > 2 || c < 0 || c > 2) continue;
        if(board->get_bigboard_cell(r, c) == '-') break;

        if(player->get_type() != PlayerType::COMPUTER)
            cout << "Cell occupied, choose another.\n";
    }
    return {r, c};
}