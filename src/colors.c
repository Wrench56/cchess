#include <ncurses.h>

#include "settings.h"

void init_colors() {
    start_color();
    use_default_colors();

    init_pair(1, WHITE_FIGURE_COLOR, WHITE_SQUARE_COLOR);
    init_pair(2, WHITE_FIGURE_COLOR, BLACK_SQUARE_COLOR);
    init_pair(3, BLACK_FIGURE_COLOR, WHITE_SQUARE_COLOR);
    init_pair(4, BLACK_FIGURE_COLOR, BLACK_SQUARE_COLOR);

    init_pair(5, COLOR_RED, -1);
    init_pair(6, COLOR_CYAN, -1);
    init_pair(7, COLOR_GREEN, -1);
    init_pair(8, COLOR_YELLOW, -1);
    
    init_pair(9, WHITE_SQUARE_COLOR, -1);
    init_pair(10, BLACK_SQUARE_COLOR, -1);
}
