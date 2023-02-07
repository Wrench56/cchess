#include <ncurses.h>
#include <locale.h>

#include "menu.h"
#include "board.h"

int main()
{
    // Support unicode //
    setlocale(LC_ALL, "");

	initscr();
    display_menu();
    draw_board();
    getch();
    endwin();

	return 0;
}
