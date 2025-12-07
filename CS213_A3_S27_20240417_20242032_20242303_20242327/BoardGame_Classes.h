#ifndef BOARDGAME_CLASSES_H
#define BOARDGAME_CLASSES_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

/////////////////////////////////////////////////////////////
// Forward declarations
/////////////////////////////////////////////////////////////

template <typename T> class Player;
template <typename T> class Move;

/////////////////////////////////////////////////////////////
// Class declarations
/////////////////////////////////////////////////////////////

/**
 * @brief Represents the type of player in the game.
 */
enum class PlayerType {
    HUMAN,     ///< A human player.
    COMPUTER,  ///< A computer-controlled player (Greedy AI in your case).
    AI,        ///< An AI player (Minimax/Alpha-Beta in your case).
    RANDOM     ///< A Random player.
};

/**
 * @brief Base template for any board used in board games.
 */
template <typename T>
class Board {
protected:
    int rows;
    int columns;
    vector<vector<T>> board;
    int n_moves = 0;

public:

    Board(int rows, int columns)
        : rows(rows), columns(columns), board(rows, vector<T>(columns)) {}

    virtual ~Board() = default;

    virtual bool update_board(Move<T>* move) = 0;

    virtual bool is_win(Player<T>*) = 0;

    virtual bool is_lose(Player<T>*) = 0;

    virtual bool is_draw(Player<T>*) = 0;

    virtual bool game_is_over(Player<T>*) = 0;

    /**
     * @brief Get the content of a specific cell (تعديل جديد).
     */
    T get_cell(int x, int y) const {
        return board[x][y];
    }

    /**
     * @brief Return a copy of the current board as a 2D vector.
     */
    vector<vector<T>> get_board_matrix() const {
        return board;
    }

    int get_rows() const { return rows; }

    int get_columns() const { return columns; }

    int get_n_moves() const { return n_moves; }
};

//-----------------------------------------------------

template <typename T>
class Move {
    int x;
    int y;
    T symbol;

public:
    Move(int x, int y, T symbol) : x(x), y(y), symbol(symbol) {}

    int get_x() const { return x; }

    int get_y() const { return y; }

    T get_symbol() const { return symbol; }
};

//-----------------------------------------------------

template <typename T>
class Player {
protected:
    string name;
    PlayerType type;
    T symbol;
    Board<T>* boardPtr;

public:
    Player(string n, T s, PlayerType t)
        : name(std::move(n)), symbol(s), type(t), boardPtr(nullptr) {}

    virtual ~Player() = default;

    string get_name() const { return name; }

    PlayerType get_type() const { return type; }

    T get_symbol() const { return symbol; }

    Board<T>* get_board_ptr() const { return boardPtr; }

    void set_board_ptr(Board<T>* b) { boardPtr = b; }
};

//-----------------------------------------------------
/**
 * @brief Base class for handling user interface and input/output.
 */
template <typename T>
class UI {
protected:
    int cell_width;

    /**
     * @brief Ask the user for the player's name.
     */
    virtual string get_player_name(const string& player_label) {
        string name;
        cout << "Enter " << player_label << " name: ";
        getline(cin >> ws, name);
        return name;
    }

    /**
     * @brief Ask the user to choose the player type from a list.
     */
    virtual PlayerType get_player_type_choice(const string& player_label,
                                              const vector<string>& options) {
        cout << "Choose " << player_label << " type:\n";
        for (size_t i = 0; i < options.size(); ++i)
            cout << i + 1 << ". " << options[i] << "\n";
        int choice;
        cout<<"Please Choose type of player: ";
        cin >> choice;

        // يعتمد هذا الجزء على الخيارات المعروضة
        if (choice == 2) return PlayerType::COMPUTER;
        // إذا كان هناك خيارات أخرى في مشروعك (Human, Computer, AI, Random)
        // يجب أن تقوم بتعديل هذا الجزء ليتوافق معها بناءً على ترتيب عرض الخيارات
        return PlayerType::HUMAN;
    }

public:
    /**
     * @brief Default constructor (تعديل جديد).
     */
    explicit UI(int cell_display_width = 3) : cell_width(cell_display_width) {} ; // 🚀 إضافة Constructor افتراضي

    /**
     * @brief Construct the UI and display a welcome message.
     */
    UI(const string& message, int cell_display_width)
        : cell_width(cell_display_width) {
        cout << message << endl;
    }

    virtual ~UI() = default; // 🚀 أصبحت virtual

    /** @brief Display any message to the user. */
    virtual void display_message(const string& message) { cout << message << "\n"; } // 🚀 أصبحت virtual

    /**
     * @brief Ask the user (or AI) to make a move.
     */
    virtual Move<T>* get_move(Player<T>*) = 0;

    /**
     * @brief Set up players for the game.
     */
    virtual Player<T>** setup_players(); // 🚀 أصبحت virtual

    /**
     * @brief Create a player object based on input name, symbol and type.
     */
    virtual Player<T>* create_player(const string& name, T symbol, PlayerType type) = 0;

    /**
     * @brief Display the current board matrix in formatted form.
     */
    virtual void display_board_matrix(const vector<vector<T>>& matrix) const { // 🚀 أصبحت virtual
        if (matrix.empty() || matrix[0].empty()) return;

        int rows = matrix.size();
        int cols = matrix[0].size();

        cout << "\n    ";
        for (int j = 0; j < cols; ++j)
            cout << setw(cell_width + 1) << j;
        cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";

        for (int i = 0; i < rows; ++i) {
            cout << setw(2) << i << " |";
            for (int j = 0; j < cols; ++j)
                cout << setw(cell_width) << matrix[i][j] << " |";
            cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
        }
        cout << endl;
    }
};

//-----------------------------------------------------

template <typename T>
Player<T>** UI<T>::setup_players() {
    auto players = new Player<T>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string nameX = get_player_name("Player X");
    PlayerType typeX = get_player_type_choice("Player X", type_options);
    players[0] = create_player(nameX, static_cast<T>('X'), typeX);

    string nameO = get_player_name("Player O");
    PlayerType typeO = get_player_type_choice("Player O", type_options);
    players[1] = create_player(nameO, static_cast<T>('O'), typeO);

    return players;
}

#endif // BOARDGAME_CLASSES_H
