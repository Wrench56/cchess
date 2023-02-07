#include <ncurses.h>

void draw_board() {
    for (short i = 0; i < 8; i++) {
        for (short j = 0; j < 8; j++) {
            move(i, j);
            if ((i+j) % 2 == 0) {
                printw("\U0001F600");
                continue;
            }
            printw("\U0001F600");

        }
    }
}
