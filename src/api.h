#pragma once

#include <cjson/cJSON.h>

cJSON* get_request(char* api_key, char* url);
void read_game_stream(void* game_state_ptr);