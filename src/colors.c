#include <ncurses.h>

#include "settings.h"

void init_colors() {
    start_color();
    use_default_colors();

    init_pair(1, WHITE_FIGURE_COLOR, WHITE_SQUARE_COLOR);
    init_pair(2, WHITE_FIGURE_COLOR, BLACK_SQUARE_COLOR);
    init_pair(3, BLACK_FIGURE_COLOR, WHITE_SQUARE_COLOR);
    init_pair(4, BLACK_FIGURE_COLOR, BLACK_SQUARE_COLOR);
}
