#include <ncurses.h>

WINDOW* create_subwin(char* title) {
    int win_w;
    int win_h;
    getmaxyx(stdscr, win_h, win_w);
    WINDOW *win = newwin(win_h-1, win_w, 1, 0);
    refresh();
    box(win, 0, 0);
    mvwprintw(win, 0, 3, " %s ", title);

    return win;
}

void sw_error(WINDOW* win, char* message) {
    /* Report error */
    wattron(win, COLOR_PAIR(5));
    mvwprintw(win, 9, 1, "Error: ");
    wattroff(win, COLOR_PAIR(5));
    printw("%s", message);
}

void sw_warning(WINDOW* win, char* message) {
    /* Report error */
    wattron(win, COLOR_PAIR(8));
    mvwprintw(win, 9, 1, "Warning: ");
    wattroff(win, COLOR_PAIR(8));
    printw("%s", message);
}