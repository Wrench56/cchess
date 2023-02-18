#pragma once

#include <ncurses.h>

#define WHITE_SQUARE_COLOR COLOR_CYAN
#define BLACK_SQUARE_COLOR COLOR_MAGENTA

#define WHITE_FIGURE_COLOR COLOR_WHITE
#define BLACK_FIGURE_COLOR COLOR_BLACK

#if defined(USE_FIGURES) 
    #define PAWN_FIGURE "♙"
    #define KNIGHT_FIGURE "♘"
    #define BISHOP_FIGURE "♗"
    #define ROOK_FIGURE "♖"
    #define QUEEN_FIGURE "♕"
    #define KING_FIGURE "♔"

    #define FIGURE_STRING "%s "
#else
    #define PAWN_FIGURE "P"
    #define KNIGHT_FIGURE "K"
    #define BISHOP_FIGURE "B"
    #define ROOK_FIGURE "R"
    #define QUEEN_FIGURE "Q"
    #define KING_FIGURE "K"
    
    #define FIGURE_STRING " %s"
#endif

