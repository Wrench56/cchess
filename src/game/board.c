#include <ncurses.h>
#include <stdlib.h>

#include "../settings.h"

#include "game.h"


void show_board(struct Game* game, int x, int y) {
    char char_to_process;
    short is_black = 0;
    short is_black_square = 1;

    short row = 1;
    /* Draw ranks & files (numbers and characters) */

    mvprintw(y + 8, x + 3, "A B C D E F G H");
    for (short i = 0; i < 8; i++) {
        mvprintw(y + i, x, "%c", (i + 49));
    }

    move(y, x + 2);

    for (short i = 0; i < 8; i++) {
        for (short j = 0; j < 8; j++) {
            if (game->is_black == 1) {
                char_to_process = game->board[63-(i*8+j)];
            } else {
                char_to_process = game->board[i*8+j];
            }            

            if (char_to_process >= 'a') {
                char_to_process -= 32;
                is_black = 2;
            }
            if (is_black_square == 0) {
                is_black_square = 1;
                attroff(COLOR_PAIR(1));
                attroff(COLOR_PAIR(3));
                attron(COLOR_PAIR(2+is_black));
            } else {
                is_black_square = 0;
                attroff(COLOR_PAIR(2));
                attroff(COLOR_PAIR(4));
                attron(COLOR_PAIR(1+is_black));
            }

            switch (char_to_process) {
                case 'P': // Pawn
                    printw(FIGURE_STRING, PAWN_FIGURE);
                    break;
                case 'R': // Rook
                    printw(FIGURE_STRING, ROOK_FIGURE);
                    break;
                case 'B': // Bishop
                    printw(FIGURE_STRING, BISHOP_FIGURE);
                    break;
                case 'N': // Knight
                    printw(FIGURE_STRING, KNIGHT_FIGURE);
                    break;
                case 'Q': // Queen
                    printw(FIGURE_STRING, QUEEN_FIGURE);
                    break;
                case 'K': // King
                    printw(FIGURE_STRING, KING_FIGURE);
                    break;
                case '0': // Space
                    printw(FIGURE_STRING, " ");
                    break;
                default:
                    printw("Er");
                    break;
            }
            is_black = 0;
        }
        is_black_square = !is_black_square;
        move(y + row, x + 2);
        row++;
    }

    /* Cleanup */
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(3));
}


void parse_move(struct Game* game, char* move_pair) {
    short rank = 56 - (move_pair[1] - 49) * 8;
    short file = move_pair[0] - 97;
    char piece = game->board[rank + file];
    game->board[rank + file] = '0';

    rank = 56 - (move_pair[3] - 49) * 8;
    file = move_pair[2] - 97;
    game->board[rank + file] = piece;
}
