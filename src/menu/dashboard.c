#include <ncurses.h>
#include <cjson/cJSON.h>

#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "../api.h"
#include "../settings.h"

#include "../game/join.h"

#include "cipher.h"

void error(char* message) {
    /* Report error */
    clear();

    attron(COLOR_PAIR(5));
    mvprintw(0, 1, "Error: ");
    attroff(COLOR_PAIR(5));
    printw("%s", message);
    getch();

    return;
}

void process_ratings(cJSON* game_type, char* game_type_name, char* symbol) {
    /* Used to display different type control infos. WARNING: Move cursor before calling this! */
    cJSON* rating = cJSON_GetObjectItemCaseSensitive(game_type, "rating");

    attron(COLOR_PAIR(7));
    if (symbol != "") {
        printw("%s ", symbol);
    }
    printw("%s  ", game_type_name);
    attroff(COLOR_PAIR(7));
    printw("%i", rating->valueint);
}

void redraw_cursor(int cursor, int prev_cursor) {
    mvprintw(prev_cursor, 18, " ");
    mvprintw(cursor, 18, ">");
}

short show_dashboard(char* api_key) {
    clear();
    curs_set(0);

    cJSON* json;
    json = get_request(api_key, "https://lichess.org/api/account");

    /* Error */

    cJSON* fail = cJSON_GetObjectItemCaseSensitive(json, "fail");
    if (fail->valueint == 1) {
        error("Invalid request!");
        return 1;
    }

    int win_w;
    int win_h;
    getmaxyx(stdscr, win_h, win_w);
    
    /* Show welcome message */

    cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "username");
    if (name == NULL) {
        error("Invalid JSON @dash::username");
        return 1;
    }
    
    mvprintw(0, (int)((win_w-strlen(name->valuestring)-8)/2), "Welcome ");
    attron(COLOR_PAIR(6));
    printw("%s", name->valuestring);
    attroff(COLOR_PAIR(6));
    printw("!");

    move(1, 0);
    for (int i = 0; i < win_w; i++) printw("=");

    /* =====================*/

    /* Show currently playing */
    short is_playing = 0;

    cJSON *playing = cJSON_GetObjectItemCaseSensitive(json, "playing");
    if (playing == NULL) {
        mvprintw(2, 1, "In menu");
    } else {
        is_playing = 1;
        attron(COLOR_PAIR(7));
        mvprintw(2, 1, "In game");
        attroff(COLOR_PAIR(7));
    }

    /* Show last seen */
    cJSON* seen_at = cJSON_GetObjectItemCaseSensitive(json, "seenAt");
    if (seen_at == NULL) {
        error("Invalid JSON @dash::seen_at");
        return 1;
    }

    time_t t = (long)seen_at->valuedouble/1000; // convert ms -> sec

    struct tm lt;
    char res[20];

    localtime_r(&t, &lt);
    strftime(res, sizeof(res), "%F", &lt);
    mvprintw(2, 18,"Last seen: %s", res);

    /* Show ratings */
    cJSON* perfs = cJSON_GetObjectItemCaseSensitive(json, "perfs");
    
    move(4, 1); process_ratings(cJSON_GetObjectItemCaseSensitive(perfs, "blitz"), "Blitz  ", BLITZ_SYMBOL);
    move(5, 1); process_ratings(cJSON_GetObjectItemCaseSensitive(perfs, "bullet"), "Bullet ", BULLET_SYMBOL);
    move(6, 1); process_ratings(cJSON_GetObjectItemCaseSensitive(perfs, "correspondence"), "Mailing", CORRESPONDENCE_SYMBOL);
    move(7, 1); process_ratings(cJSON_GetObjectItemCaseSensitive(perfs, "classical"), "Classic", CLASSICAL_SYMBOL);
    move(8, 1); process_ratings(cJSON_GetObjectItemCaseSensitive(perfs, "rapid"), "Rapid  ", RAPID_SYMBOL);


    /* Show "play buttons" */

    short cursor = 5;

    if (is_playing == 1) {
        cursor = 4;
        mvprintw(4, 20, "Join");
    }
    mvprintw(5, 20, "New Game");
    mvprintw(6, 20, "Challenge");

    mvprintw(cursor, 18, ">");

    /* Wait for key inputs*/

    char key = '\0';

    while (key != 'q') {
        key = getch();
        if (key == '\n') {
            switch (cursor-4) {
                case 0: // Join
                    if (join(api_key) == 1) {
                        return 0;
                    }
                    break;
                case 1: // New Game
                    break;
                case 2: // Challenge (friend)
                    break;
            }
        } else if (key == 's') {
            if (cursor < 6) {
                cursor++;
                redraw_cursor(cursor, cursor - 1);
            }
        } else if (key == 'w') {
            if (cursor > 5 - is_playing) {
                cursor--;
                redraw_cursor(cursor, cursor + 1);
            }
        } else if (key == '?' || key == 'h') { // Show help
            WINDOW *win = newwin(win_h-1, win_w, 1, 0);
            refresh();

            box(win, 0, 0);
            mvwprintw(win, 0, 3, " Help ");

            mvwprintw(win, 1, 2, "w - (Nav) Up");
            mvwprintw(win, 2, 2, "s - (Nav) Down");
            mvwprintw(win, 3, 2, "? - Help (or h)");
            mvwprintw(win, 4, 2, "q - Quit");
            mvwprintw(win, 5, 2, "-- Menu specials --");
            mvwprintw(win, 6, 2, "@ - .autologin user");
            wrefresh(win);

            while (getch() != 'q');
	        delwin(win);

            touchwin(stdscr);
            wrefresh(stdscr);
        } else if (key == '@') { // Set .autologin
            mvprintw(10, 1, "Set .autologin? [y/n] >");
            curs_set(1);
            if (getch() == 'y') {
                FILE *file;
                char* encrypted_token = NULL;
                encrypted_token = xor_cipher(api_key, AUTOLOGIN_KEY, strlen(api_key), AUTOLOGIN_KEY_LEN);
                file = fopen(".autologin", "w");
                fputs(encrypted_token, file);

                fclose(file);
                free(encrypted_token);
            }
            curs_set(0);
            mvprintw(10, 1, "                       ");
        }
    }

    return 1;
}