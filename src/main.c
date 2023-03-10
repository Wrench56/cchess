#include <ncurses.h>
#include <locale.h>

#include "colors.h"
#include "menu/login.h"

int main()
{
    // Support unicode //
    setlocale(LC_ALL, "");

	initscr();
    init_colors();
    noecho();

    login();
    endwin();

	return 0;
}
