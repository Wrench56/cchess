#pragma once

struct Game {
    char api_key[25]; 
    char game_id[9];
    char board[65];
    char white_name[32];
    char black_name[32];
    short is_black;
};

void game_stream(char* api_key, char* game_id);