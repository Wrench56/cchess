#include <ncurses.h>
#include <pthread.h>
#include <string.h>

#include "../api.h"

#include "game.h"



/* API thread */
void* read_api(void* game_struct_ptr) {
    read_game_stream(game_struct_ptr);
}

/* Main thread (Input thread) */
void game_stream(char* api_key, char* game_id) {
    /* Set up API thread */
    pthread_t api_thread;
    struct Game game;

    strcpy(game.api_key, api_key);
    strcpy(game.game_id, game_id);

    /* Set starting board */
    strcpy(game.board, "rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR");

    pthread_create(&api_thread, NULL, read_api, &game);
    
    /* Setup UI */
    clear();
    
    refresh();
    getch();

}