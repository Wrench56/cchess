#include <ncurses.h>
#include <locale.h>

#include "board.h"
#include "colors.h"
#include "menu.h"

int main()
{
    // Support unicode //
    setlocale(LC_ALL, "");

	initscr();
    init_colors();

    display_menu();

    char basic_fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

    draw_fen(0, 0, basic_fen, sizeof(basic_fen));
    getch();
    endwin();

	return 0;
}
