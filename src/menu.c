#include <ncurses.h>

void display_menu() {
    move(1, 1);

    printw("Welcome to cchess!");
    getch();
}
