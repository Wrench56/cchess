#include <ncurses.h>
#include <stdlib.h>

#include "../settings.h"

#include "game.h"


#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))


void show_board(struct Game* game, int x, int y) {
    char char_to_process;
    short is_black = 0;
    short is_black_square = 1;

    short row = 1;
    /* Draw ranks & files (numbers and characters) */

    if (game->is_black) {
        mvprintw(y + 8, x + 3, "H G F E D C B A");
        for (short i = 8; i > 0; i--) {
            mvprintw(y + i - 1, x, "%c", (i + 48));
        }
    } else {
        mvprintw(y + 8, x + 3, "A B C D E F G H");
        for (short i = 0; i < 8; i++) {
            mvprintw(y + i, x, "%c", (i + 49));
        }
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
                case '@': // Possible move
                    if (is_black_square == 0) {
                        attron(COLOR_PAIR(11));
                    } else {
                        attron(COLOR_PAIR(12));
                    }
                    printw(FIGURE_STRING, POSSIBLE_MOVE_STRING);
                    if (game->is_black == 1) {
                        game->board[63-(i*8+j)] = '0';
                    } else {
                        game->board[i*8+j] = '0';
                    }            
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

void show_valid_moves(struct Game* game, char* piece_location) {
    short is_black = 0;
    unsigned short state;

    short rank = 56 - (piece_location[1] - 49) * 8;
    short file = piece_location[0] - 97;
    char piece = game->board[rank + file];
    

    if (piece >= 'a') {
        piece -= 32;
        is_black = 1;
    }

    switch (piece) {
        case 'P': // Pawn
            if (!is_black) {
                if (game->board[rank+file-8] == '0') {
                    game->board[rank + file - 8] = '@';
                    if (rank == 48 && !is_black) { // 2. rank
                        if (game->board[rank+file-16] == '0') {
                            game->board[rank + file - 16] = '@'; // @-sign: valid move for selected piece
                        }
                    }
                }
            } else {
                if (game->board[rank+file+8] == '0') {
                    game->board[rank + file + 8] = '@';
                    if (rank == 8 && is_black) {
                        if (game->board[rank+file+16] == '0') {
                            game->board[rank + file + 16] = '@'; // @-sign: valid move for selected piece
                        }
                    }
                }
            }
            break;

        case 'R': // Rook
            state = 15;

            for (short i = 1; i < 8; i++) {
                /* Horizontal */
                if (CHECK_BIT(state, 0)) {
                    if (file+i < 8 && game->board[rank+file+i] == '0') {
                        game->board[rank+file+i] = '@';
                    } else {
                        state -= 1;
                    }
                }
                if (CHECK_BIT(state, 1)) {
                    if (file+i > -1 && game->board[rank+file-i] == '0') {
                        game->board[rank+file-i] = '@';
                    } else {
                        state -= 2;
                    }
                }

                /* Vertical */
                if (CHECK_BIT(state, 2)) {
                    if (rank + i*8 < 63 && game->board[rank+file+i*8] == '0') {
                        game->board[rank+file+i*8] = '@';
                    } else {
                        state -= 4;
                    }
                }
                if (CHECK_BIT(state, 3)) {
                    if (rank - i*8 > -1 && game->board[rank+file-i*8] == '0') {
                        game->board[rank+file-i*8] = '@';
                    } else {
                        state -= 8;
                    }
                }

                /* Break when everything is done! */
                if (!state) break;
            }
            break;
        case 'B': // Bishop
            state = 15;

            for (short i = 1; i < 8; i++) {
                if (CHECK_BIT(state, 0)) {
                    if (rank+file+i*7 < 64 && game->board[rank+file+i*7] == '0') {
                        game->board[rank+file+i*7] = '@';
                        if ((rank+file+i*7) % 8 == 0) {
                            state -= 1;
                        } else if ((rank+file+i*7) % 8 == 0) {
                            state -= 1;
                        }
                    } else {
                        state -= 1;
                    }
                }
                if (CHECK_BIT(state, 1)) {
                    if (rank+file+i*9 < 64 && game->board[rank+file+i*9] == '0') {
                        game->board[rank+file+i*9] = '@';
                        if ((rank+file+1+i*9) % 8 == 0) {
                            state -= 2;
                        } else if ((rank+file+i*9) % 8 == 0) {
                            state -= 2;
                        }
                    } else {
                        state -= 2;
                    }
                }
                if (CHECK_BIT(state, 2)) {
                    if (rank+file-i*7 > -1 && game->board[rank+file-i*7] == '0') {
                        game->board[rank+file-i*7] = '@';
                        if ((rank+file+1-i*7) % 8 == 0) {
                            state -= 4;
                        } else if ((rank+file-i*7) % 8 == 0) {
                            state -= 4;
                        }
                    } else {
                        state -= 4;
                    }
                }                
                if (CHECK_BIT(state, 3)) {
                    if (rank+file-i*9 > -1 && game->board[rank+file-i*9] == '0') {
                        game->board[rank+file-i*9] = '@';
                        if ((rank+file+1-i*9) % 8 == 0) {
                            state -= 8;
                        } else if ((rank+file-i*9) % 8 == 0) {
                            state -= 8;
                        }
                    } else {
                        state -= 8;
                    }
                }

                /* Break when everything is done! */
                if (!state) break;
            }
            break;

        case 'Q':
            state = 15;

            state = 15;

            for (short i = 1; i < 8; i++) {
                /* Horizontal */
                if (CHECK_BIT(state, 0)) {
                    if (file+i < 8 && game->board[rank+file+i] == '0') {
                        game->board[rank+file+i] = '@';
                    } else {
                        state -= 1;
                    }
                }
                if (CHECK_BIT(state, 1)) {
                    if (file+i > -1 && game->board[rank+file-i] == '0') {
                        game->board[rank+file-i] = '@';
                    } else {
                        state -= 2;
                    }
                }

                /* Vertical */
                if (CHECK_BIT(state, 2)) {
                    if (rank + i*8 < 63 && game->board[rank+file+i*8] == '0') {
                        game->board[rank+file+i*8] = '@';
                    } else {
                        state -= 4;
                    }
                }
                if (CHECK_BIT(state, 3)) {
                    if (rank - i*8 > -1 && game->board[rank+file-i*8] == '0') {
                        game->board[rank+file-i*8] = '@';
                    } else {
                        state -= 8;
                    }
                }

                /* Break when everything is done! */
                if (!state) break;
            }
            state = 15;
            
            for (short i = 1; i < 8; i++) {
                if (CHECK_BIT(state, 0)) {
                    if (rank+file+i*7 < 64 && game->board[rank+file+i*7] == '0') {
                        game->board[rank+file+i*7] = '@';
                        if ((rank+file+i*7) % 8 == 0) {
                            state -= 1;
                        } else if ((rank+file+i*7) % 8 == 0) {
                            state -= 1;
                        }
                    } else {
                        state -= 1;
                    }
                }
                if (CHECK_BIT(state, 1)) {
                    if (rank+file+i*9 < 64 && game->board[rank+file+i*9] == '0') {
                        game->board[rank+file+i*9] = '@';
                        if ((rank+file+1+i*9) % 8 == 0) {
                            state -= 2;
                        } else if ((rank+file+i*9) % 8 == 0) {
                            state -= 2;
                        }
                    } else {
                        state -= 2;
                    }
                }
                if (CHECK_BIT(state, 2)) {
                    if (rank+file-i*7 > -1 && game->board[rank+file-i*7] == '0') {
                        game->board[rank+file-i*7] = '@';
                        if ((rank+file+1-i*7) % 8 == 0) {
                            state -= 4;
                        } else if ((rank+file-i*7) % 8 == 0) {
                            state -= 4;
                        }
                    } else {
                        state -= 4;
                    }
                }                
                if (CHECK_BIT(state, 3)) {
                    if (rank+file-i*9 > -1 && game->board[rank+file-i*9] == '0') {
                        game->board[rank+file-i*9] = '@';
                        if ((rank+file+1-i*9) % 8 == 0) {
                            state -= 8;
                        } else if ((rank+file-i*9) % 8 == 0) {
                            state -= 8;
                        }
                    } else {
                        state -= 8;
                    }
                }

                /* Break when everything is done! */
                if (!state) break;
            }
            break;
    }
}