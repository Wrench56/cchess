#include <ncurses.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#include "../api.h"

#include "board.h"
#include "game.h"

void report(char* message) {
    int win_w;
    int win_h;
    getmaxyx(stdscr, win_h, win_w);

    int msg_len = 0;

    /* Support string literals for strtok() */
    char *msg_copy = (char *)malloc(strlen(message) + 1);
    strcpy(msg_copy, message);

    char* word = strtok(msg_copy, " ");

    while (word != NULL) {
        if ((strlen(word) + msg_len) > (win_w - 33)) {
            move(10, 21);
            msg_len = 0;
        }
        printw(" %s", word);
        msg_len += strlen(word);
        word = strtok(NULL, " ");
    }
}

void report_cleanup() {
    int win_w;
    int win_h;
    getmaxyx(stdscr, win_h, win_w);

    for (int i = 0; i < win_w; i++) {
        mvaddch(9, 22+i, ' ');
        mvaddch(10, 22+i, ' ');
    }
}

void report_error(char* message) {
    report_cleanup();
    attron(COLOR_PAIR(5));
    mvprintw(9, 22, "Error:");
    attroff(COLOR_PAIR(5));
    report(message);
}

void report_success(char* message) {
    report_cleanup();
    attron(COLOR_PAIR(7));
    mvprintw(9, 22, "Success:");
    attroff(COLOR_PAIR(7));
    report(message);
}

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
            if (strlen(move_inp) == 2) { // Select
                show_valid_moves(&game, move_inp);
                show_board(&game, 1, 1);
                refresh();

            } else if (strlen(move_inp) == 4) { // Move
                char joined_url[54] = "https://lichess.org/api/board/game/";
                strcat(joined_url, game.game_id);
                strcat(joined_url, "/move/");
                strcat(joined_url, move_inp);

                cJSON* returned = http_request(game.api_key, joined_url, "");
                cJSON* ok = cJSON_GetObjectItemCaseSensitive(returned, "ok");
                if (ok == NULL) {
                    cJSON* error = cJSON_GetObjectItemCaseSensitive(returned, "error");
                    report_error(error->valuestring);
                } else {
                    report_success("Piece moved!");
                }
                
            } else { // Error
                report_error("Invalid move/select!");
            }
            
        }
        if (game.change_flag >= 1) {
            /* Handle change */
            show_board(&game, 1, 1);
            refresh();

            /* Visible/audible notification */
            #ifdef USE_BEEP
                beep();
            #endif

            #ifdef USE_FLASH
                flash();
            #endif


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