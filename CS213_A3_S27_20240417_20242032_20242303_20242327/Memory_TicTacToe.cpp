#include "Memory_TicTacToe.h"
#include <cstdlib>
#include <ctime>

// -------------------- Constructor --------------------
Memory_TicTacToe::Memory_TicTacToe() : Board<char>(3,3), n_moves(0) {
    board = vector<vector<char>>(3, vector<char>(3,'-'));
}

// -------------------- Update --------------------
bool Memory_TicTacToe::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char sym = move->get_symbol();
    if(r<0 || r>2 || c<0 || c>2) return false;
    if(board[r][c]!='-') return false;

    board[r][c] = sym;
    n_moves++;
    return true;
}

// -------------------- Check win --------------------
bool Memory_TicTacToe::is_win(Player<char>* player) {
    char sym = player->get_symbol();
    for(int i=0;i<3;i++){
        if(board[i][0]==sym && board[i][1]==sym && board[i][2]==sym) return true;
        if(board[0][i]==sym && board[1][i]==sym && board[2][i]==sym) return true;
    }
    if(board[0][0]==sym && board[1][1]==sym && board[2][2]==sym) return true;
    if(board[0][2]==sym && board[1][1]==sym && board[2][0]==sym) return true;
    return false;
}

bool Memory_TicTacToe::is_lose(Player<char>* player) {
    char other = (player->get_symbol()=='X'?'O':'X');
    for(int i=0;i<3;i++){
        if(board[i][0]==other && board[i][1]==other && board[i][2]==other) return true;
        if(board[0][i]==other && board[1][i]==other && board[2][i]==other) return true;
    }
    if(board[0][0]==other && board[1][1]==other && board[2][2]==other) return true;
    if(board[0][2]==other && board[1][1]==other && board[2][0]==other) return true;
    return false;
}

bool Memory_TicTacToe::is_draw(Player<char>* player) {
    return n_moves==9 && !is_win(player) && !is_lose(player);
}

bool Memory_TicTacToe::game_is_over(Player<char>* player) {
    return is_win(player) || is_lose(player) || is_draw(player);
}

// -------------------- Display board --------------------
void Memory_TicTacToe::display_board() {
    cout << "\nBoard (marks hidden):\n   0 1 2\n";
    for(int i=0;i<3;i++){
        cout << i << " |";
        for(int j=0;j<3;j++){
            cout << '-' << " |";  // Always hide marks
        }
        cout << "\n";
    }
}

char Memory_TicTacToe::get_cell(int r,int c){
    return board[r][c];
}

// -------------------- UI --------------------
MemoryUI::MemoryUI() : UI("Memory Tic Tac Toe",2) { srand(time(0)); }

Move<char>* MemoryUI::get_move(Player<char>* player){
    Memory_TicTacToe* b = dynamic_cast<Memory_TicTacToe*>(player->get_board_ptr());
    if(player->get_type()==PlayerType::COMPUTER){
        int r,c;
        do { r=rand()%3; c=rand()%3; } while(b->get_cell(r,c)!='-');
        cout << "Computer plays: " << r << " " << c << "\n";
        return new Move<char>(r,c,player->get_symbol());
    } else {
        int r,c;
        cout << player->get_name() << " (" << player->get_symbol() << ") enter row col (0..2): ";
        cin >> r >> c;
        return new Move<char>(r,c,player->get_symbol());
    }
}


Player<char>* MemoryUI::create_player(string& name,char symbol,PlayerType type){
    return new Player<char>(name,symbol,type);
}
