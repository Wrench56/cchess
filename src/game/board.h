#pragma once

struct RemainingPieces {
    uint white_pawns:4;
    uint white_knights:2;
    uint white_bishops:2;
    uint white_rooks:2;
    uint white_queen:1;

    uint black_pawns:4;
    uint black_knights:2;
    uint black_bishops:2;
    uint black_rooks:2;
    uint black_queen:1;
};

struct Game;
void show_board(struct Game* game, int x, int y);
void parse_move(struct Game* game, char* move_pair);
void show_valid_moves(struct Game* game, char* piece_location);

struct RemainingPieces get_remaining_pieces(struct Game* game);
void print_taken_pieces(int x, int y, struct RemainingPieces r_pieces, short is_black);
