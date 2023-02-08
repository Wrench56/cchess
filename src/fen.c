#include <ncurses.h>
#include <stdlib.h>

#include "settings.h"


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
