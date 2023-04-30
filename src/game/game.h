#pragma once

struct Message {
    char username[32];
    char text[141];
};

struct Game {
    char api_key[25]; 
    char game_id[9];
    char board[65];
    char white_name[32];
    char black_name[32];
    short is_black;
    unsigned int num_moves;
    unsigned short change_flag;
    struct Message* message;
};

void game_stream(char* api_key, char* game_id);
