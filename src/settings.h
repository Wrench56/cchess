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

#if defined(USE_SYMBOLS)
    #define BLITZ_SYMBOL ""
    #define BULLET_SYMBOL ""
    #define CORRESPONDENCE_SYMBOL ""
    #define CLASSICAL_SYMBOL ""
    #define RAPID_SYMBOL ""
#else
    #define BLITZ_SYMBOL ""
    #define BULLET_SYMBOL ""
    #define CORRESPONDENCE_SYMBOL ""
    #define CLASSICAL_SYMBOL ""
    #define RAPID_SYMBOL ""
#endif

#define AUTOLOGIN_KEY "cchess_key"
#define AUTOLOGIN_KEY_LEN sizeof(AUTOLOGIN_KEY)
