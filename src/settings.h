#pragma once

#include <ncurses.h>

#define WHITE_SQUARE_COLOR COLOR_CYAN
#define BLACK_SQUARE_COLOR COLOR_MAGENTA

#define WHITE_FIGURE_COLOR COLOR_WHITE
#define BLACK_FIGURE_COLOR COLOR_BLACK

#define POSSIBLE_SQUARE_BG_WHITE WHITE_SQUARE_COLOR
#define POSSIBLE_SQUARE_BG_BLACK BLACK_SQUARE_COLOR
#define POSSIBLE_SQUARE_FG COLOR_YELLOW

#define TARGET_SQUARE_BACKGROUND COLOR_RED
#define TARGET_SQUARE_FG_WHITE WHITE_FIGURE_COLOR
#define TARGET_SQUARE_FG_BLACK BLACK_FIGURE_COLOR

#define CHARACTER_PER_PIECE 1

#if defined(USE_FIGURES) 
    #define PAWN_FIGURE "\u2659" // \u265F is sometimes an emoji
    #define KNIGHT_FIGURE "\u265E"
    #define BISHOP_FIGURE "\u265D"
    #define ROOK_FIGURE "\u265C"
    #define QUEEN_FIGURE "\u265B"
    #define KING_FIGURE "\u265A"

    #if (CHARACTER_PER_PIECE == 1)
        #define FIGURE_STRING(i) (i == 7 ? "%s " : "%s ")
    #else
        #define FIGURE_STRING(i) (i == 7 ? "%s" : "%s ")
    #endif

#else
    #define PAWN_FIGURE "P"
    #define KNIGHT_FIGURE "N"
    #define BISHOP_FIGURE "B"
    #define ROOK_FIGURE "R"
    #define QUEEN_FIGURE "Q"
    #define KING_FIGURE "K"
    
    #define FIGURE_STRING(i) (i ? " %s" : " %s")
#endif

#if defined(USE_SYMBOLS)
    #define BLITZ_SYMBOL ""
    #define BULLET_SYMBOL ""
    #define CORRESPONDENCE_SYMBOL ""
    #define CLASSICAL_SYMBOL ""
    #define RAPID_SYMBOL ""

    #define POSSIBLE_MOVE_STRING ""
    #define POSSIBLE_MOVE_FORMAT " %s"

#else
    #define BLITZ_SYMBOL ""
    #define BULLET_SYMBOL ""
    #define CORRESPONDENCE_SYMBOL ""
    #define CLASSICAL_SYMBOL ""
    #define RAPID_SYMBOL ""

    #define POSSIBLE_MOVE_STRING "*"
    #define POSSIBLE_MOVE_FORMAT "%c "
    
#endif

#define AUTOLOGIN_KEY "cchess_key"
#define AUTOLOGIN_KEY_LEN sizeof(AUTOLOGIN_KEY)
