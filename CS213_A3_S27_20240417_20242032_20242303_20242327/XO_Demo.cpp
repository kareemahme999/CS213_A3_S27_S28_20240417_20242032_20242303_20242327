#include <iostream> // Required for input/output operations (cout, cin)
#include <string>   // Required for string
#include <vector>   // Required for vector
#include <memory>   // Required for unique_ptr
#include <limits> // Required for clearing input buffer
#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "SUS.h"
#include "Numerical_Tic_Tac_Toe.h"
#include "FiveXO.h"
#include "FourXO.h"
#include "Misere_Tic_Tac_Teo.h"
#include "Diamond.h"
#include "Four_in_a_row.h"
#include "Obstacles.h"
#include "Pyramid_Board.h"
#include "Infinity_Board.h"
#include "Word.h"
#include "Ultimate_TicTacToe.h"
#include "Memory_TicTacToe.h"


using namespace std;

/*──────▄▌▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
 * ───▄▄██▌█ beep beep--------------
 * ▄▄▄▌▐██▌█ -KAREEM_AHMED------------
 * ███████▌█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌
 * ▀(@)▀▀▀▀(@)(@)▀▀▀▀▀▀▀▀▀▀▀(@)▀-----------------------
 *
*/
/*
*******# Name of Team Member:  #*******

Name:Kareem Ahmed>>>>ID:20240417:
>>Task_1:
>>Task_2:

Name:Arwa Amin>>>>ID:20242032:
>>Task_3:
>>Task_4:

Name:Mohamed Mortada>>>>ID:20242303
>>Task_5:
>>Task_6:

Name:Maryam Sayed>>>>ID:20242327:
>>Task_7:
>>Task_8:

###ALL Task of Group:###
>>Task_9:
>>Task_10:
>>Task_11:
>>Task_12:

###Bonus Task of Group:###
>>Task_13:
*/













void clear_cin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}



void menu() {
    cout << "-------------------------------------------\n";
    cout << "\n--- 🎮 Welcome to the FCAI Game Center 🎮 ---\n";
    cout << "Individual Games:\n";
    cout << "0 : Play Classic X-O Game\n";
    cout << "1 : Play SUS\n";
    cout << "2 : Play Four-in-a-row\n";
    cout << "3 : 5 x 5 Tic Tac Toe\n";
    cout << "4 : Play Word Tic-tac-toe\n";
    cout << "5 : Play Misère Tic Tac Toe\n";
    cout << "6 : Play Diamond Tic-Tac-Toe\n";
    cout << "7 : Play 4 x 4 Tic-Tac-Toe\n";
    cout << "8 : Play Pyramid Tic-Tac-Toe\n";
    cout << "-------------------------------------------\n";
    cout << "Group Games:\n";
    cout << "9 : Play Numerical Tic-Tac-Toe\n";
    cout << "10: Play Obstacles Tic-Tac-Toe\n";
    cout << "11: Play Infinity Tic-Tac-Toe\n";
    cout << "12: Play Ultimate Tic Tac Toe\n";
    cout << "-------------------------------------------\n";
    cout <<"Bonus Games:\n";
    cout <<"13: Play Memory Tic-Tac-Toe\n";
    cout <<"14: Exit\n";
    cout << "-------------------------------------------\n";
    cout <<"please enter your choice from the menu(0>>>>14):";
}

void xo_game() {
    cout << "\n===============================\n";
    cout << "  Starting Classic X-O Game  \n";
    cout << "===============================\n" ;

    // Create an instance of the specific UI for X-O using a pointer
    UI<char>* game_ui = new XO_UI();

    // Create the game board. For X-O, this is an X_O_Board.
    Board<char>* xo_board = new X_O_Board();

    // Use the UI to set up the players for the game.
    // The UI returns a dynamically allocated array of Player pointers.
    Player<char>** players = game_ui->setup_players();

    // Create the game manager with the board and the array of players.
    GameManager<char> x_o_game(xo_board, players, game_ui);

    // Run the game loop.
    x_o_game.run();

    // --- Cleanup ---
    // Delete the dynamically allocated board object.
    delete xo_board;
    delete game_ui;
    // Delete the individual player objects.
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    // Delete the dynamically allocated array of player pointers itself.
    delete[] players;

cout << "\n--- X-O Game Over ---\n";
}

void run_numerical_game() {
    cout << "\n=====================================\n";
    cout << "  Starting Numerical Tic-Tac-Toe  \n";
    cout << "=====================================\n";

    // Create an instance of the specific UI for X-O using a pointer
    UI<int>* game_ui = new Numerical_UI();

    // Create the game board. For X-O, this is an X_O_Board.
    Board<int>* num_board = new Numerical_Board();



    // Use the UI to set up the players for the game.
    // The UI returns a dynamically allocated array of Player pointers.
    Player<int>** players = game_ui->setup_players();



    // Create the game manager with the board and the array of players.
    GameManager<int> num_game(num_board, players, game_ui);

    // Run the game loop.
    num_game.run();

    // --- Cleanup ---
    // Delete the dynamically allocated board object.
    delete num_board;
    delete game_ui;
    // Delete the individual player objects.
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    // Delete the dynamically allocated array of player pointers itself.
    delete[] players;

    cout << "\n--- Numerical Game Over ---\n";

}
void run_sus_game() {
    cout << "\n=====================================\n";
    cout << "       Starting SUS Game  \n";
    cout << "  Goal: Create S-U-S sequences!  \n";
    cout << "=====================================\n";

    UI<char>* game_ui = new SUS_UI();
    SUS_Board* sus_board = new SUS_Board();
    Player<char>** players = game_ui->setup_players();

    // 2. Create and Run Game Manager
    GameManager<char> sus_game(sus_board, players, game_ui);
    sus_game.run();

    // 3. Display Final Results
    cout << "\n--- Final Board State ---\n";
    sus_board->display_board();

    // Display final scores
    cout << "\n--- Final SUS Count ---\n";
    cout << players[0]->get_name() << ">>>Player1 : "
         << sus_board->get_player1_score() << " total SUS sequences\n";
    cout << players[1]->get_name() << ">>>Player2 : "
         << sus_board->get_player2_score() << " total SUS sequences\n";

    if (sus_board->get_player1_score() > sus_board->get_player2_score()) {
        cout << "\n WINNER: " << players[0]->get_name() << " \n";
    } else if (sus_board->get_player2_score() > sus_board->get_player1_score()) {
        cout << "\n WINNER: " << players[1]->get_name() << " \n";
    } else {
        cout << "\nIt's a TIE in score!\n";
        cout << " DRAW! No winner declared. \n";
    }

    // 4. Cleanup
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;
    delete sus_board;
    delete game_ui;

    cout << "\n--- SUS Game Over ---\n";
}

void run_four_in_a_row_game() {
    cout << "\n=====================================\n";
    cout << "  Starting Four-in-a-row Game (6x7) \n";
    cout << "=====================================\n";

    UI<char>* game_ui = new FourInARow_UI();

    Board<char>* fir_board = new FourInARow_Board(); // FIR = Four In a Row

    Player<char>** players = game_ui->setup_players();

    GameManager<char> fir_game(fir_board, players, game_ui);

    fir_game.run();


    delete fir_board;
    delete game_ui;

    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;

    cout << "\n--- Four-in-a-row Game Over ---\n";
}
//misereXO
void misere_game() {
    cout << "\n===============================\n";
    cout << "   Starting Misère Tic Tac Toe\n";
    cout << "===============================\n";

    // Create UI for Misère Tic Tac Toe
    UI<char>* game_ui = new MiserUI();

    // Create Misère board
    Board<char>* misere_board = new MiserBoard();

    // Setup players
    Player<char>** players = game_ui->setup_players();

    // Create game manager
    GameManager<char> misere_game(misere_board, players, game_ui);

    // Run the game
    misere_game.run();

    // --- Cleanup ---
    delete misere_board;
    delete game_ui;

    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;

    cout << "\n--- Misère Tic Tac Toe Game Over ---\n";
}
void Diamond_game() {
    cout << "\n===============================\n";
    cout << "   Starting Diamond Tic Tac Toe\n";
    cout << "===============================\n";

    // Create UI for Diamond Tic Tac Toe
    UI<char>* game_ui = new DiamondUI();

    // Create Diamond board
    Board<char>* diamond_board = new DiamondBoard();

    // Setup players
    Player<char>** players = game_ui->setup_players();

    // Create game manager
    GameManager<char> diamond_game(diamond_board, players, game_ui);

    // Run the game
    diamond_game.run();

    // --- Cleanup ---
    delete diamond_board;
    delete game_ui;

    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;

    cout << "\n--- Diamond Tic Tac Toe Game Over ---\n";
}
//fiveXO
void run_5x5_tic_tac_toe() {
    cout << "\n=== Starting 5x5 Tic Tac Toe (4 in a row) ===\n";
    UI<char>* game_ui = new XO_UI();
    Board<char>* five_board = new FiveXOBoard();
    Player<char>** players = game_ui->setup_players();
    GameManager<char> game(five_board, players, game_ui);
    game.run();
    delete five_board;
    delete game_ui;
    for (int i = 0; i < 2; i++) delete players[i];
    delete[] players;
    cout << "\n--- Game Over ---\n";
}

//fourXO
void run_4x4_game() {
    cout << "\n=====================================\n";
    cout << "    Starting 4x4 MOVING Tic-Tac-Toe  \n";
    cout << "=====================================\n";
    cout << "Rules:\n";
    cout << "- Each player has 4 pieces\n";
    cout << "- Move your pieces to adjacent empty cells\n";
    cout << "- Win by getting 3 pieces in a row\n";
    cout << "- Pieces can move up, down, left, right (not diagonal)\n\n";

    // Initialize random seed
    srand(static_cast<unsigned int>(time(0)));

    Board_4x4* board = new Board_4x4();
    UI_4x4* ui = new UI_4x4();

    // Setup players
    Player<char>** players = ui->setup_players();

    // Create and run game manager
    GameManager<char> game_manager(board, players, ui);
    game_manager.run();

    // Cleanup
    delete board;
    delete ui;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;

    cout << "\n--- 4x4 Moving Game Over ---\n";
}

void play_pyramid_game() {
    cout << "\nStarting Pyramid Tic-Tac-Toe...\n";

    Pyramid_Board* board = new Pyramid_Board();
    Pyramid_UI* ui = new Pyramid_UI();
    Player<char>** players = ui->setup_players();

    GameManager<char> gameManager(board, players, ui);
    gameManager.run();

    delete board;
    delete ui;
    delete players[0];
    delete players[1];
    delete[] players;

    cout << "Game finished.\n\n";
}


void run_obstacles_game() {
    cout << "\n=====================================\n";
    cout << "     Starting Obstacles Tic Tac Toe (6x6)\n";
    cout << "   Goal: Get 4 in a row while obstacles appear!\n";
    cout << "=====================================\n";

    UI<char>* game_ui = new Obstacles_UI();
    Board<char>* board = new ObstaclesBoard();
    Player<char>** players = game_ui->setup_players();

    GameManager<char> game(board, players, game_ui);
    game.run();

    // cleanup
    for (int i = 0; i < 2; i++) delete players[i];
    delete[] players;
    delete board;
    delete game_ui;

    cout << "\n--- Obstacles Tic Tac Toe Game Over ---\n";
}


void play_infinity_game() {
    cout << "\n🎮 Starting Infinity Tic-Tac-Toe...\n";
    cout << "💡 Remember: Oldest moves disappear every 3 turns!\n\n";

    Infinity_Board* board = new Infinity_Board();
    Infinity_UI* ui = new Infinity_UI();
    Player<char>** players = ui->setup_players();

    GameManager<char> gameManager(board, players, ui);
    gameManager.run();

    delete board;
    delete ui;
    delete players[0];
    delete players[1];
    delete[] players;

    cout << "Infinity game finished.\n\n";
}

void run_word_game() {
    cout << "\n===== Starting Word Tic-Tac-Toe =====\n";

    UI<char>* ui = new WordUI();
    Board<char>* board = new WordBoard();
    Player<char>** players = ui->setup_players();

    GameManager<char> game(board, players, ui);
    game.run();

    for (int i = 0; i < 2; i++) delete players[i];
    delete[] players;
    delete board;
    delete ui;

    cout << "\n--- Word Game Over ---\n";
}

void play_ultimate_tictactoe() {
    UltimateUI* ui = new UltimateUI();
    Ultimate_TicTacToe* board = new Ultimate_TicTacToe();

    // Setup players
    Player<char>** players = ui->setup_players();
    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    int turn = 0; // Player 0 starts

    while(true){
        Player<char>* current = players[turn];

        // 0) Display Big Board first
        cout << "\n--- Big Board ---\n";
        board->display_big_board();

        // 1) Choose a free cell in big_board
        pair<int,int> big_pos = ui->choose_bigboard_cell(current, board);
        int br = big_pos.first;
        int bc = big_pos.second;

        cout << current->get_name() << " selected Big Board cell: (" << br << "," << bc << ")\n";

        // 2) Reset and play the sub-board
        board->reset_subboard();

        // Display sub-board directly
        board->display_subboard();

        while(true){
            // Get move
            Move<char>* move = ui->get_move(current);
            while(!board->update_board(move)) {
                cout << "Invalid move, try again.\n";
                delete move;
                move = ui->get_move(current);
            }

            board->display_subboard();

            // Check win/draw in sub-board
            if(board->is_win(current)){
                cout << current->get_name() << " wins this sub-board!\n";
                board->set_bigboard_cell(br, bc, current->get_symbol());
                break;
            }
            if(board->is_draw(current)){
                cout << "Sub-board draw!\n";
                board->set_bigboard_cell(br, bc, '-');
                break;
            }

            // Switch player inside sub-board
            turn = 1 - turn;
            current = players[turn];
        }

        // 3) Display big_board after sub-board
        cout << "\n--- Big Board After Sub-board ---\n";
        board->display_big_board();

        // 4) Check win in big_board
        for(int i=0;i<2;i++){
            if(board->get_bigboard_cell(br,bc)==players[i]->get_symbol()){
                char sym = players[i]->get_symbol();
                bool win=false;
                // Horizontal
                for(int r=0;r<3;r++)
                    if(board->get_bigboard_cell(r,0)==sym && board->get_bigboard_cell(r,1)==sym && board->get_bigboard_cell(r,2)==sym) win=true;
                // Vertical
                for(int c=0;c<3;c++)
                    if(board->get_bigboard_cell(0,c)==sym && board->get_bigboard_cell(1,c)==sym && board->get_bigboard_cell(2,c)==sym) win=true;
                // Diagonals
                if(board->get_bigboard_cell(0,0)==sym && board->get_bigboard_cell(1,1)==sym && board->get_bigboard_cell(2,2)==sym) win=true;
                if(board->get_bigboard_cell(0,2)==sym && board->get_bigboard_cell(1,1)==sym && board->get_bigboard_cell(2,0)==sym) win=true;

                if(win){
                    cout << "\nCongratulations! " << players[i]->get_name() << " wins the Ultimate Tic Tac Toe!\n";
                    // Cleanup
                    delete players[0]; delete players[1]; delete[] players; delete ui; delete board;
                    return;
                }
            }
        }

        // 5) Switch player for next sub-board
        turn = 1 - turn;
    }
}

void play_memory_tictactoe() {
    MemoryUI* ui = new MemoryUI();
    Memory_TicTacToe* board = new Memory_TicTacToe();

    Player<char>** players = ui->setup_players();
    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    int turn = 0;

    while(true){
        Player<char>* current = players[turn];

        board->display_board();

        Move<char>* move = ui->get_move(current);
        while(!board->update_board(move)){
            cout << "Invalid move, try again.\n";
            delete move;
            move = ui->get_move(current);
        }

        // Switch player
        turn = 1 - turn;

        if(board->is_win(current)){
            cout << "\n" << current->get_name() << " wins the Memory Tic Tac Toe!\n";
            break;
        }
        if(board->is_draw(current)){
            cout << "\nGame Draw!\n";
            break;
        }
    }

    delete players[0]; delete players[1]; delete[] players; delete ui; delete board;
}





// ====================================================================
int main() {
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator

bool run_games=true;
while (run_games) {
    int choice=-1;
    menu();
    cin >> choice;
    if (cin.fail()) {
        cout << "Invalid input. Please enter a number.\n";
        clear_cin();
        continue;
    }
    switch (choice) {
        /*******Individual Games***********/

        case 0: {
            xo_game();
            break;
        }
            /******************/
        case 1: {
            run_sus_game();
            break;
        }
            /******************/

        case 2: {
            run_four_in_a_row_game();
            break;
        }
            /******************/

        case 3: {
            run_5x5_tic_tac_toe();
            break;
        }
            /******************/

        case 4: {
            run_word_game();
            break;
        }
            /******************/

        case 5: {
            misere_game();
            break;
        }
            /******************/
        case 6: {
            Diamond_game();
            break;
        }
            /******************/

        case 7: {
            run_4x4_game();
            break;
        }
            /******************/

        case 8: {
            play_pyramid_game();
            break;
        }
            /*******Group Games***********/

        case 9: {
            run_numerical_game();
            break;
        }
            /******************/

        case 10: {
            run_obstacles_game();
            break;
        }
            /******************/

        case 11: {
            play_infinity_game();
            break;
        }
            /******************/
        case 12: {
            play_ultimate_tictactoe();
            break;
        }
            /******************/
        case 13: {
            play_memory_tictactoe();
            break;
        }
            /******************/
        case 14: {
            run_games = false;
            cout << "Thank you for playing. Exiting...\n";
            break;
        }
        default: {
            cout << "Invalid choice. Please select from the menu options.\n";
            break;
        }
    }
}



return 0;
}
