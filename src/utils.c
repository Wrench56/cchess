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
