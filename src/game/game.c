#include <ncurses.h>
#include <pthread.h>
#include <string.h>

#include "../api.h"

#include "game.h"



/* API thread */
void* read_api(void* game_state_ptr) {
    read_game_stream(game_state_ptr);
}

/* Main thread (Input thread) */
void game_stream(char* api_key, char* game_id) {
    /* Set up API thread */
    pthread_t api_thread;
    struct GameState game_state;

    strcpy(game_state.api_key, api_key);
    strcpy(game_state.game_id, game_id);

    pthread_create(&api_thread, NULL, read_api, &game_state);
    
    /* Setup UI */
    clear();
    
    refresh();
    getch();

}