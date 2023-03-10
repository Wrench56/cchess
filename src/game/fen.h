#pragma once

#include <stdio.h>

struct FEN {
    char piece_placement[86];       // The fen for pieces (should be 85)
    short piece_placement_length;   // Length of piece_placement
    short active_color;             // 0 for white, 1 for black
    short castle_white;             // 0: can't castle, +1: king side, +2: queen side
    short castle_black;             // Same
    char en_passant[2];             // En passant target square
    short halfmove;                 // Fifty-move rule
    int fullmove;                   // Fullmoves
};


struct FEN parse_fen(char fen_string[], size_t fen_size);
void draw_fen(int x, int y, char fen_table[], size_t fen_size);
