#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "fen.h"
#include "settings.h"

enum FENSM {
    PiecePlacement,
    ActiveColor,
    Castle,
    EnPassant,
    Halfmove,
    Fullmove
};

struct FEN parse_fen(char fen_string[], size_t fen_size) {
    enum FENSM FENState = PiecePlacement;
    struct FEN fen;
    
    short moves_start = 0;
    char moves[4]; // Longest theoretically possible game is 5949 moves
    
    // Set some defaults // 

    fen.castle_white = 0;
    fen.castle_black = 0;

    for (short i = 0; i < fen_size; i++) {
        switch (FENState) {
            case PiecePlacement:
                if (fen_string[i] != ' ') {
                    continue;
                }
                memcpy(fen.piece_placement, fen_string, i);
                fen.piece_placement_length = i;
                FENState++;

                break;

            case ActiveColor:
                if (fen_string[i] == 'w') {
                    fen.active_color = 0;
                } else {
                    fen.active_color = 1;
                }
                i++;
                FENState++;

                break;

            case Castle:
                if (fen_string[i] == 'K') {
                    fen.castle_white++;
                } else if (fen_string[i] == 'Q') {
                    fen.castle_white += 2;
                } else if (fen_string[i] == 'k') {
                    fen.castle_black++;
                } else if (fen_string[i] == 'q') {
                    fen.castle_black += 2;
                } else {
                    FENState++;
                }

                break;
            case EnPassant:
                if (fen_string[i] != '-') {
                    strncat(fen.en_passant, &fen_string[i], 2);
                    i++;
                } else {
                    fen.en_passant[0] = '-';
                    fen.en_passant[1] = '-';
                }
                i++;
                FENState++;

                break;
            case Halfmove:
                if (moves_start == 0) {
                    moves_start = i;
                }
                if (fen_string[i] != ' ') {
                    moves[i-moves_start] = fen_string[i];
                } else {
                    fen.halfmove = atoi(moves);
                    moves[0] = 0;
                    moves_start = 0;

                    FENState++;
                }
        
                break;
            case Fullmove:
                
                if (moves_start == 0) {
                    moves_start = i;
                }
                // Ignore \0
                if (fen_string[i] != '\0') {
                    moves[i-moves_start] = fen_string[i];
                }
                break;

            default:
                break;
        }
        fen.fullmove = atoi(moves);
    }

    return fen;
}

void draw_fen(int x, int y, char fen_table[], size_t fen_size) {
    short is_black_square = 1;
    short is_black = 0;

    short x_offset = 0;
    short y_offset = 0;

    char char_to_process;
        
    clear();
    refresh();
    move(y, x);
    
    if (fen_table[0] >= 'a') {
        is_black = 2;
    }
    attron(COLOR_PAIR(1));

    for (int i = 0; i < fen_size; i++) {
        is_black = 0;

        // Skip x squares OR "newline"
        if (fen_table[i] < 'A') {
            move(y + y_offset, x + x_offset);

            if (fen_table[i] == '/') {
                y_offset++;
                x_offset = 0;
                move(y + y_offset, x);
                
                if (is_black_square == 0) {
                    is_black_square = 1;
                    attroff(COLOR_PAIR(1));
                    attron(COLOR_PAIR(2));
                } else {
                    is_black_square = 0;
                    attroff(COLOR_PAIR(2));
                    attron(COLOR_PAIR(1));
                }
            }
            
            for (int j = 0; j < (fen_table[i] - '0'); j++) {
                if (is_black_square == 0) {
                    is_black_square = 1;
                    attroff(COLOR_PAIR(1));
                    attron(COLOR_PAIR(2));
                } else {
                    is_black_square = 0;
                    attroff(COLOR_PAIR(2));
                    attron(COLOR_PAIR(1));
                }
                printw("  ");
                x_offset += 2;

                move(y + y_offset, x + x_offset);

            }
        
        } else {
            // Set the colors
            
            if (fen_table[i] >= 'a') {
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
            char_to_process = fen_table[i];
            if (char_to_process >= 'a') {
                char_to_process -= 32;
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
                default:
                    printw("Er");
                    break;
            }

            x_offset += 2;

        }
    }
    move(y + y_offset, x + x_offset);
    refresh();
}
