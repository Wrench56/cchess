#include <ncurses.h>
#include <pthread.h>
#include <string.h>

#include "../api.h"

#include "board.h"
#include "game.h"



/* API thread */
void* read_api(void* game_struct_ptr) {
    read_game_stream(game_struct_ptr);
}

/* Main thread (Input thread & output) */
void game_stream(char* api_key, char* game_id) {
    /* Set up API thread */
    pthread_t api_thread;
    struct Game game;

    /* Copy existing data */
    strcpy(game.api_key, api_key);
    strcpy(game.game_id, game_id);
    
    /* Set default values */
    game.is_black = 2;
    game.change_flag = 0;

    /* Set starting board */
    strcpy(game.board, "rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR");
    pthread_create(&api_thread, NULL, read_api, &game);
    
    /* Setup UI */
    clear();
    refresh();

    while (game.is_black == 2) // block until data arrives

    if (game.is_black == 1) {
        mvprintw(0, 2, "%s", game.white_name);
    } else {
        mvprintw(0, 2, "%s", game.black_name);
    }
    mvprintw(10, 2, "You");
    mvprintw(7, 22, ">");
    show_board(&game, 1, 1);
    refresh();

    /* Mainloop */
    timeout(1);
    char key = '\0';
    short current_pos = 0;
    char move_inp[5];

    while (1) {
        key = getch();
        if (key == 'q') {
            break;
        }
        if ((key >= '0' && key <= '9') || (key >='A' && key <= 'H') || (key >= 'a' && key <= 'h')) {
            /* New select/move via keyboard */
            mvprintw(7, 24 + current_pos, "%c", key);
            move_inp[current_pos] = key;
            current_pos++;

            refresh();
        } else if (key == '\b' || key == 127 || key == KEY_BACKSPACE) {
            if (current_pos > 0) {
                current_pos--;
                move_inp[current_pos] = '\0';
                mvprintw(7, 24 + current_pos, " ");

                refresh();
            }
        } else if (key == '\n') {
            if (strlen(move_inp) == 2) { // Move select
                show_valid_moves(&game, move_inp);
                show_board(&game, 1, 1);
                refresh();

            } else if (strlen(move_inp) == 4) { // Move

            } else { // Error
                
            }
            
        }
        if (game.change_flag >= 1) {
            /* Handle change */
            show_board(&game, 1, 1);
            refresh();

            game.change_flag = 0;
        }
    }

    /* Cleanup */
    timeout(-1);

    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
    attroff(COLOR_PAIR(3));
    attroff(COLOR_PAIR(4));

}