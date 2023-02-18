#include <ncurses.h>
#include <locale.h>

#include "fen.h"
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

    //char basic_fen[] = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";
    //struct FEN fen;

    //fen = parse_fen(basic_fen, sizeof(basic_fen));
    //draw_fen(2, 1, fen.piece_placement, fen.piece_placement_length);
    endwin();

	return 0;
}
