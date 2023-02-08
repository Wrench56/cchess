#include <ncurses.h>
#include <stdlib.h>

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
            printf("%c", char_to_process);
    
            switch (char_to_process) {
                case 'P': // Pawn
                    printw(" P");
                    break;
                case 'R': // Rook
                    printw(" R");
                    break;
                case 'B': // Bishop
                    printw(" B");
                    break;
                case 'N': // Knight
                    printw(" N");
                    break;
                case 'Q': // Queen
                    printw(" Q");
                    break;
                case 'K': // King
                    printw(" K");
                    break;
                default:
                    printw("Er");
                    break;
            }

            x_offset += 2;

        }


        if (fen_table[i+1] >= 'a') {
            is_black = 2;
        }
        
    }
    move(y + y_offset, x + x_offset);
    refresh();
}
