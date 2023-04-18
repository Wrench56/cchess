#include <ncurses.h>
#include <stdlib.h>

#include "../settings.h"

#include "game.h"


#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define KNIGHT_MULTIPLIER_CALC(i) ((i > 1) ? 16 : 8)
#define KING_MULTIPLIER_CALC(i) ((i == 0) ? 0 : 8)

const short KNIGHT_OFFSETS[4] = {6, 10, 15, 17};
const short KING_OFFSETS[4] = {1, 7, 8, 9};


void show_board(struct Game* game, int x, int y) {
    char char_to_process;
    short is_black = 0;
    short is_black_square = 1;
    short target_square = 0;

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
            mvprintw(y + 7 - i, x, "%c", (i + 49));
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
                
            /* Target flag set */
            if (CHECK_BIT(char_to_process, 7)) {
                target_square = 1;
                char_to_process -= 128;
            }

            if (char_to_process >= 'a') {
                char_to_process -= 32;
                is_black = 2;
            }
            

            if (!target_square) {
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
            } else {
                attron(COLOR_PAIR(13+is_black));
                is_black_square = !is_black_square;
                target_square = 0;
                /* Clear target squares*/
                if (game->is_black == 1) {
                    game->board[63-(i*8+j)] = char_to_process + is_black * 16; // Got to add 32 if is_black == 2; (2 = true)
                } else {
                    game->board[i*8+j] = char_to_process + is_black * 16;
            }
            }

            switch (char_to_process) {
                case 'P': // Pawn
                    printw(FIGURE_STRING(j), PAWN_FIGURE);
                    break;
                case 'R': // Rook
                    printw(FIGURE_STRING(j), ROOK_FIGURE);
                    break;
                case 'B': // Bishop
                    printw(FIGURE_STRING(j), BISHOP_FIGURE);
                    break;
                case 'N': // Knight
                    printw(FIGURE_STRING(j), KNIGHT_FIGURE);
                    break;
                case 'Q': // Queen
                    printw(FIGURE_STRING(j), QUEEN_FIGURE);
                    break;
                case 'K': // King
                    printw(FIGURE_STRING(j), KING_FIGURE);
                    break;
                case '0': // Space
                    printw(FIGURE_STRING(0), " ");
                    break;
                case '@': // Possible move
                    if (is_black_square == 0) {
                        attron(COLOR_PAIR(11));
                    } else {
                        attron(COLOR_PAIR(12));
                    }
                    printw(POSSIBLE_MOVE_FORMAT, POSSIBLE_MOVE_STRING);
                    if (game->is_black == 1) {
                        game->board[63-(i*8+j)] = '0';
                    } else {
                        game->board[i*8+j] = '0';
                    }            
                    break;
                default:
                    printw("%i", char_to_process);
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


short set_target(char attacking_figure, char* target_position) {
    /* Check if the square fed to target_position is an actual target or
       a field where it can move                                         */

    if (*target_position == '0') {
        *target_position = '@';
        return 0;
    }

    short is_black = 0;
    if (*target_position >= 'a') {
        is_black = 1;
    }

    if (!(attacking_figure >= 'a' && is_black) && !((attacking_figure < 'a' && !is_black))) {
        *target_position = (1 << 7) | *target_position;
    }
    return 1;
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
                if (file > 0 && game->board[rank+file-9] > 'a' && game->board[rank+file-9] < 'z') {
                    game->board[rank+file-9] = (1 << 7) | game->board[rank+file-9];   
                }
                if (file < 7 && game->board[rank+file-7] > 'a' && game->board[rank+file-7] < 'z') {
                    game->board[rank+file-7] = (1 << 7) | game->board[rank+file-7];   
                }

                if (game->board[rank+file-8] == '0') {
                    game->board[rank+file-8] = '@';
                    if (rank == 48 && !is_black) { // 2. rank
                        if (game->board[rank+file-16] == '0') {
                            game->board[rank+file-16] = '@'; // @-sign: valid move for selected piece
                        }
                    }
                }
            } else {
                if (file < 7 && game->board[rank+file+9] > 'A' && game->board[rank+file+9] < 'Z') {
                    game->board[rank+file+9] = (1 << 7) | game->board[rank+file+9];   
                }
                if (file > 0 && game->board[rank+file+7] > 'A' && game->board[rank+file+7] < 'Z') {
                    game->board[rank+file+7] = (1 << 7) | game->board[rank+file+7];   
                }

                if (game->board[rank+file+8] == '0') {
                    game->board[rank+file+8] = '@';
                    if (rank == 8 && is_black) {
                        if (game->board[rank+file+16] == '0') {
                            game->board[rank+file+16] = '@'; // @-sign: valid move for selected piece
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
                    if (i < 8-file) {
                        if (set_target(game->board[rank+file], &game->board[rank+file+i])) {
                            state--;
                        }
                    } else {
                        state--;
                    }
                }
                if (CHECK_BIT(state, 1)) {
                    if (-i >= -(file % 8)) {
                        if (set_target(game->board[rank+file], &game->board[rank+file-i])) {
                            state -= 2;
                        }
                    } else {
                        state -= 2;
                    }
                }

                /* Vertical */
                if (CHECK_BIT(state, 2)) {
                    if (rank + i*8 < 63) {
                        if (set_target(game->board[rank+file], &game->board[rank+file+i*8])) {
                            state -= 4;
                        }
                    } else {
                        state -= 4;
                    }
                }
                if (CHECK_BIT(state, 3)) {
                    if (rank - i*8 > -1) {
                        if (set_target(game->board[rank+file], &game->board[rank+file-i*8])) {
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
        case 'B': // Bishop
            state = 15;

            for (short i = 1; i < 8; i++) {
                if (CHECK_BIT(state, 0)) {
                    if (i*7 >= 8-file+(i-1)*8) {
                        if (set_target(game->board[rank+file], &game->board[rank+file+i*7])) {
                            state--;
                        }
                    } else {
                        state--;
                    }
                }
                if (CHECK_BIT(state, 1)) {
                    if (i*9 < 8-file+(i)*8) {
                        if (set_target(game->board[rank+file], &game->board[rank+file+i*9])) {
                            state -= 2;    
                        }
                    } else {
                        state -= 2;
                    }
                }
                if (CHECK_BIT(state, 2)) {
                    if (-i*7 < -(file%8)-(i-1)*8 && rank+file-i*7 > -1) {
                        if (set_target(game->board[rank+file], &game->board[rank+file-i*7])) {
                            state -= 4;
                        }
                    } else {
                        state -= 4;
                    }
                }    
                
                if (CHECK_BIT(state, 3)) {
                    if ((-i*9 >= -(file%8)-(i)*8)) {
                        if (set_target(game->board[rank+file], &game->board[rank+file-i*9])) {
                            state -= 8;
                        }
                    }
                }

                /* Break when everything is done! */
                if (!state) break;
            }
            break;

        case 'Q':
            state = 15;

            for (short i = 1; i < 8; i++) {
                /* Horizontal */
                if (CHECK_BIT(state, 0)) {
                    if (i < 8-file) {
                        if (set_target(game->board[rank+file], &game->board[rank+file+i])) {
                            state--;
                        }
                    } else {
                        state--;
                    }
                }
                if (CHECK_BIT(state, 1)) {
                    if (-i >= -(file % 8)) {
                        if (set_target(game->board[rank+file], &game->board[rank+file-i])) {
                            state -= 2;
                        }
                    } else {
                        state -= 2;
                    }
                }

                /* Vertical */
                if (CHECK_BIT(state, 2)) {
                    if (rank + i*8 < 63) {
                        if (set_target(game->board[rank+file], &game->board[rank+file+i*8])) {
                            state -= 4;
                        }
                    } else {
                        state -= 4;
                    }
                }
                if (CHECK_BIT(state, 3)) {
                    if (rank - i*8 > -1) {
                        if (set_target(game->board[rank+file], &game->board[rank+file-i*8])) {
                            state -= 8;
                        }
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
                    if (i*7 >= 8-file+(i-1)*8) {
                        if (set_target(game->board[rank+file], &game->board[rank+file+i*7])) {
                            state--;
                        }
                    } else {
                        state--;
                    }
                }
                if (CHECK_BIT(state, 1)) {
                    if (i*9 < 8-file+(i)*8) {
                        if (set_target(game->board[rank+file], &game->board[rank+file+i*9])) {
                            state -= 2;    
                        }
                    } else {
                        state -= 2;
                    }
                }
                if (CHECK_BIT(state, 2)) {
                    if (-i*7 < -(file%8)-(i-1)*8 && rank+file-i*7 > -1) {
                        if (set_target(game->board[rank+file], &game->board[rank+file-i*7])) {
                            state -= 4;
                        }
                    } else {
                        state -= 4;
                    }
                }    
                
                if (CHECK_BIT(state, 3)) {
                    if ((-i*9 >= -(file%8)-(i)*8)) {
                        if (set_target(game->board[rank+file], &game->board[rank+file-i*9])) {
                            state -= 8;
                        }
                    }
                }

                /* Break when everything is done! */
                if (!state) break;
            }
            break;

        case 'K': // King
            for (short i = 0; i < 4; i++) {
                if (KING_OFFSETS[i] >= -(file%8)+KING_MULTIPLIER_CALC(i) &&
                        KING_OFFSETS[i] < 8-file+KING_MULTIPLIER_CALC(i) && 
                        rank+file+KING_OFFSETS[i] < 64) {
                    set_target(game->board[rank+file], &game->board[rank+file+KING_OFFSETS[i]]);
                }
                if (-KING_OFFSETS[i] >= -(file%8)-KING_MULTIPLIER_CALC(i) &&
                        -KING_OFFSETS[i] < 8-file-KING_MULTIPLIER_CALC(i) &&
                        rank+file-KING_OFFSETS[i] > -1) {
                    set_target(game->board[rank+file], &game->board[rank+file-KING_OFFSETS[i]]);
                }
                
            }
            break;
        
        case 'N': // Knight
            for (short i = 0; i < 4; i++) {
                
                if (KNIGHT_OFFSETS[i] >= -(file%8)+KNIGHT_MULTIPLIER_CALC(i) &&
                        KNIGHT_OFFSETS[i] < 8-file+KNIGHT_MULTIPLIER_CALC(i) && 
                        rank+file+KNIGHT_OFFSETS[i] < 64) {
                    set_target(game->board[rank+file], &game->board[rank+file+KNIGHT_OFFSETS[i]]);
                }
                if (-KNIGHT_OFFSETS[i] >= -(file%8)-KNIGHT_MULTIPLIER_CALC(i) &&
                        -KNIGHT_OFFSETS[i] < 8-file-KNIGHT_MULTIPLIER_CALC(i) && 
                        rank+file-KNIGHT_OFFSETS[i] > -1) {
                    set_target(game->board[rank+file], &game->board[rank+file-KNIGHT_OFFSETS[i]]);
                }
            }
            break;

        default:
            break;   
    }
}