#include <ncurses.h>
#include <stdlib.h>

void draw_fen(int x, int y, char fen_table[], size_t fen_size) {
    short is_black_square = 0;
    short is_black = 0;

    short x_offset = 0;
    short y_offset = 0;
        
    clear();
    refresh();
    move(y, x);

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
                
                continue;
            }
            
            for (int j = 0; j < (fen_table[i] - '0'); j++) {
                printw("  ");
                x_offset += 2;

                move(y + y_offset, x + x_offset);

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
            
        } else if (fen_table[i] >= 'a') {
            is_black = 2;
        }

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
    refresh();
}
