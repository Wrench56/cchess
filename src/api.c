#include <string.h>
#include <stdlib.h>

#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <ncurses.h>

#include "game/game.h"
#include "game/board.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};


size_t get_https_result(char* buffer, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, realsize + 1);
    if(!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), buffer, realsize);
    mem->size = realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


cJSON* get_request(char* api_key, char* url) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *list = NULL;
    struct MemoryStruct chunk;
 
    chunk.memory = malloc(1);   /* will be grown as needed by the realloc above */
    chunk.size = 0;             /* no data at this point */
  

    curl = curl_easy_init();
    char auth_header[47] = "Authorization: Bearer ";
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        list = curl_slist_append(list, strcat(auth_header, api_key));
        list = curl_slist_append(list, "Accept:");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_https_result);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &chunk);
 
    
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_slist_free_all(list);
        curl_easy_cleanup(curl);
    }
    
    if (res == 0) {
        cJSON* result = cJSON_Parse(chunk.memory);
        cJSON_AddNumberToObject(result, "fail", 0);
        return result;
    } else {
        return cJSON_Parse("{\"fail\": 1}");
    }
}



size_t parse_game_stream(char* buffer, size_t size, size_t nmemb, void* userp)
{
    struct Game *game = (struct Game *)userp;

    size_t realsize = size * nmemb;
    char data[realsize + 1];
    memcpy(data, buffer, realsize);
    if (*data < 2) { // Check for keep-alive
        return realsize;
    }

    cJSON* json = cJSON_Parse(data);
    if (json == NULL) {
        printw("Error: Can't parse JSON!");
        return realsize;
    }

    cJSON* type = cJSON_GetObjectItemCaseSensitive(json, "type");
    
    if (strcmp(type->valuestring, "gameState") == 0) {
        cJSON* state = cJSON_GetObjectItemCaseSensitive(json, "state");
        cJSON* moves = cJSON_GetObjectItemCaseSensitive(moves, "moves");

        moves->valuestring += strlen(moves->valuestring); // Parse only the last move!
        parse_move(game, moves->valuestring);
        show_board(game, 1, 1);

    } else if (strcmp(type->valuestring, "opponentGone") == 0) {

    } else if (strcmp(type->valuestring, "gameFull") == 0) {
        cJSON* white = cJSON_GetObjectItemCaseSensitive(json, "white");
        cJSON* w_name = cJSON_GetObjectItemCaseSensitive(white, "name");
        cJSON* black = cJSON_GetObjectItemCaseSensitive(json, "black");
        cJSON* b_name = cJSON_GetObjectItemCaseSensitive(white, "name");

        strcpy(game->white_name, w_name->valuestring);
        strcpy(game->black_name, b_name->valuestring);

        cJSON* state = cJSON_GetObjectItemCaseSensitive(json, "state");
        cJSON* moves = cJSON_GetObjectItemCaseSensitive(state, "moves");

        if (strlen(moves->valuestring) == 4) {
            parse_move(game, moves->valuestring);
        } else if (strlen(moves->valuestring) > 4) {
            // Extract the first token
            char * move_ = strtok(moves->valuestring, " ");
            parse_move(game, move_);
            // loop through the string to extract all other tokens
            while( move_ != NULL ) {
                parse_move(game, move_);
                move_ = strtok(NULL, " ");
            }
        }
        show_board(game, 1, 1);
        refresh();

    } else { // chatLine

    }

    return realsize;
}


void read_game_stream(void* game_struct_ptr) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *list = NULL;
    struct Game *game = (struct Game*)game_struct_ptr;

    curl = curl_easy_init();
    char auth_header[47] = "Authorization: Bearer ";
    char url[51] = "https://lichess.org/api/board/game/stream/";
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, strcat(url, game->game_id));
        list = curl_slist_append(list, strcat(auth_header, game->api_key));
        list = curl_slist_append(list, "Accept:");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parse_game_stream);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) game);
 
    
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_slist_free_all(list);
        curl_easy_cleanup(curl);
    }
}
