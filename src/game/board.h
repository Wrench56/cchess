#pragma once

struct Game;
void show_board(struct Game* game, int x, int y);
void parse_move(struct Game* game, char* move_pair);
void show_valid_moves(struct Game* game, char* piece_location);
