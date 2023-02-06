#include <ncurses.h>

int main()
{	
    printf("Starting cchess...");
	initscr();
	printw("Hello World !!!");	/* Print Hello World		  */
	refresh();			/* Print it on to the real screen */
	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */

	return 0;
}
