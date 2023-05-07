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

    /* Possible move */
    init_pair(11, POSSIBLE_SQUARE_FG, POSSIBLE_SQUARE_BG_WHITE);
    init_pair(12, POSSIBLE_SQUARE_FG, POSSIBLE_SQUARE_BG_BLACK);

    /* Target squares */
    init_pair(13, TARGET_SQUARE_FG_WHITE, TARGET_SQUARE_BACKGROUND);
    init_pair(15, TARGET_SQUARE_FG_WHITE, TARGET_SQUARE_BACKGROUND);

    /* Winner sign */
    init_pair(17, WHITE_FIGURE_COLOR, -1);
    init_pair(18, BLACK_FIGURE_COLOR, -1);
}
