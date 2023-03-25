#pragma once

struct Game {
    char api_key[25]; 
    char game_id[9];
    char board[65];
};

void game_stream(char* api_key, char* game_id);