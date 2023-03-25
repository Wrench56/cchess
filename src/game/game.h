#pragma once

struct GameState {
    char api_key[25]; 
    char game_id[9];
};

void game_stream(char* api_key, char* game_id);