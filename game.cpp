#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <time.h>
#include <ctype.h>

using namespace std;

#define N 3

void clear_term()
{
    system("clear");
}

void print_board(char b[N][N])
{
    for (int i = 0; i < 13; i++) {
        cout << "-";
    }
    cout << endl;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("| %c ", b[i][j]);
        }

        cout << "|" << endl;
        for (int j = 0; j < 13; j++) {
            cout << "-";
        }
        cout << endl;
    }
}

void make_player_move(char b[N][N], int row, int col, char p)
{
    if (row > 3 || col > 3 || row < 1 || col < 1) {
        cout <<  "That row or column doesn't exist.\n";
        return;
    }

    b[row-1][col-1] = p;
}

int *ask_for_position()
{
    int *pos = new int[2];
    cout << "Choose position::";

    for (int i = 0; i < 2; ++i) {
        cin >> pos[i];
    }
    // HANDLING NOT NUMERIC VALUES
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return pos;
    delete [] pos;
}

bool pos_is_valid(int *pos, char b[N][N])
{
    int row = pos[0]-1;
    int col = pos[1]-1;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (b[row][col] != ' ') {
                return false;
            }
        }
    }

    return true;
}

bool two_in_a_row(char b[N][N], char pc, char p)
{
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            if (col < N - 2 && b[row][col] == p && b[row][col + 1] == p && b[row][col + 2] != pc) {
                if (b[row][col + 2] == ' ') {
                    b[row][col + 2] = pc;
                    return true;
                }
            }

            if (row < N - 2 && b[row][col] == p && b[row + 1][col] == p && b[row + 2][col] != pc) {
                if (b[row + 2][col] == ' ') {
                    b[row + 2][col] = pc;
                    return true;
                }
            }

            if (row < N - 2 && col < N - 2 && b[row][col] == p && b[row + 1][col + 1] == p && b[row + 2][col + 2] != pc) {
                if (b[row + 2][col + 2] == ' ') {
                    b[row + 2][col + 2] = pc;
                    return true;
                }
            }

            if (row < N - 2 && col >= 2 && b[row][col] == p && b[row + 1][col - 1] == p && b[row + 2][col - 2] != pc) {
                if (b[row + 2][col - 2] == ' ') {
                    b[row + 2][col - 2] = pc;
                    return true;
                }
            }
        }
    }
    return false;
}

void make_pc_move(char b[N][N], char p)
{
    int *pos = new int[2];
    pos[0] = 2;
    pos[1] = 1;

    char player = (p == 'X') ? 'O' : 'X';
    if (b[1][1] == player) {
        pos[0] = 1;
        pos[1] = 1;
    }

    if (!two_in_a_row(b, p, player)) {
        while (!pos_is_valid(pos, b)) {
            pos[0] = (rand() % N) + 1;
            pos[1] = (rand() % N) + 1;
        }

        b[pos[0]-1][pos[1]-1] = p;
    }
}

bool win(char b[N][N], char p)
{
    if ((b[0][0] == p && b[0][1] == p && b[0][2] == p) ||
        (b[1][0] == p && b[1][1] == p && b[1][2] == p) ||
        (b[2][0] == p && b[2][1] == p && b[2][2] == p)) {
        return true;
    }

    if ((b[0][0] == p && b[1][0] == p && b[2][0] == p) ||
       (b[0][1] == p && b[1][1] == p && b[2][1] == p) ||
       (b[0][2] == p && b[1][2] == p && b[2][2] == p)) {
        return true;
    }

    if ((b[0][0] == p && b[1][1] == p && b[2][2] == p) ||
       (b[0][2] == p && b[1][1] == p && b[2][0] == p)) {
        return true;
    }

    return false;
}

int main()
{
    srand(time(NULL));

    char board[N][N] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    char player, pc;
    int player_turn;
    int moves = 0;
    bool player_did_choose = false;

    // GAME LOOP
    while (true) {
        // FIRST PLAY
        if (!player_did_choose) {
            printf("Choose\nO or X :: ");
            cin >> player;
            player = toupper(player);

            bool valid_character = (player == 'X' || player == 'O') ? true : false;

            if (!valid_character) {
                cout << "That's not an option.\n";
                continue;
            }

            pc = (player == 'X') ? 'O' : 'X';
            player_turn = (player == 'X') ? 1 : 0;
            player_did_choose = true;
        }

        // THE REST OF THE GAME
        clear_term();
        print_board(board);

        if (win(board, player)) {
            cout << "Player win.\n";
            break;
        }

        if (moves > 8) {
            cout << "Draw.\n";
            break;
        }

        if (win(board, pc)) {
            cout << "PC win.\n";
            break;
        }

        if (player_turn) {
            int *pos = ask_for_position();

            if (!pos_is_valid(pos, board)) {
                clear_term();
                cout << "Invalid position\n";
                continue;
            }

            make_player_move(board, pos[0], pos[1], player);
            player_turn = 0;
        } else {
            make_pc_move(board, pc);
            player_turn = 1;
        }
        moves++;
    }
}
