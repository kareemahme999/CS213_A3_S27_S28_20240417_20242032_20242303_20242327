#include "Word.h"
#include <fstream>
#include <iostream>

using namespace std;

WordBoard::WordBoard() : Board<char>(3, 3) {
    board.assign(3, vector<char>(3, '-'));

    ifstream fin("dic.txt");
    string w;
    while (fin >> w) {
        for (auto &c : w) c = toupper(c);
        dictionary.insert(w);
    }
}

bool WordBoard::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char s = toupper(move->get_symbol());

    if (!in_bounds(r, c)) return false;
    if (board[r][c] != '-') return false;

    board[r][c] = s;
    n_moves++;
    return true;
}

bool WordBoard::is_win(Player<char>* player) {
    // Rows
    for (int r = 0; r < 3; r++) {
        string w;
        for (int c = 0; c < 3; c++) w += board[r][c];
        if (check_word(w)) return true;
    }

    // Columns
    for (int c = 0; c < 3; c++) {
        string w;
        for (int r = 0; r < 3; r++) w += board[r][c];
        if (check_word(w)) return true;
    }

    // Diagonal 1
    string d1;
    for (int i = 0; i < 3; i++) d1 += board[i][i];
    if (check_word(d1)) return true;

    // Diagonal 2
    string d2;
    for (int i = 0; i < 3; i++) d2 += board[i][2 - i];
    if (check_word(d2)) return true;

    return false;
}

bool WordBoard::is_draw(Player<char>* p) {
    return n_moves == 9 && !is_win(p);
}

// ===============================
// UI
// ===============================
Move<char>* WordUI::get_move(Player<char>* p) {
    int r, c;
    char letter;

    if (p->get_type() == PlayerType::COMPUTER) {
        while (true) {
            r = rand() % 3;
            c = rand() % 3;
            letter = 'A' + (rand() % 26);

            if (p->get_board_ptr()->get_board_matrix()[r][c] == '-') {
                cout << p->get_name() << " (Computer) played ("
                     << r << "," << c << ") letter " << letter << "\n";
                return new Move<char>(r, c, letter);
            }
        }
    }

    cout << p->get_name() << " Enter row, column, letter: ";
    cin >> r >> c >> letter;
    return new Move<char>(r, c, letter);
}
