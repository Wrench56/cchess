#include <ncurses.h>
#include <stdlib.h>

#include "game.h"


void show_board(struct Game* game, int x, int y) {
    move(y, x);
    short row = 1;
    for (short i = 0; i < 8; i++) {
        for (short j = 0; j < 8; j++) {
            // I'm not very happy with this solution
            printw("%c", game->board[i*8+j]);
        }
        move(y+row, x);
        row++;
    }
}


void parse_move(struct Game* game, char* move_pair) {
    short rank = (move_pair[1] - 49) * 8;
    short file = move_pair[0] - 97;
    char piece = game->board[rank + file];
    game->board[rank + file] = '0';

    rank = (move_pair[3] - 49) * 8;
    file = move_pair[2] - 97;
    game->board[rank + file] = piece;
}
