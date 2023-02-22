#include <ncurses.h>
#include <cjson/cJSON.h>

#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "../api.h"
#include "../settings.h"


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
    printw("%s: ", game_type_name);
    attroff(COLOR_PAIR(7));
    printw("%i", rating->valueint);
}

void home(char* api_key) {
    clear();

    cJSON* json;
    json = get_user_data(api_key);

    /* Error */

    cJSON* fail = cJSON_GetObjectItemCaseSensitive(json, "fail");
    if (fail->valueint == 1) {
        error("Invalid request!");
        return;
    }

    int win_w;
    int win_h;
    getmaxyx(stdscr, win_h, win_w);
    
    /* Show welcome message */

    cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "username");
    if (name == NULL) {
        error("Invalid JSON");
        return;
    }
    
    mvprintw(0, (int)((win_w-strlen(name->valuestring)-8)/2), "Welcome ");
    attron(COLOR_PAIR(6));
    printw("%s", name->valuestring);
    attroff(COLOR_PAIR(6));
    printw("!");

    move(1, 0);
    for (int i = 0; i < win_w; i++) printw("=");

    //cJSON_Delete(name);
    /* =====================*/

    /* Show currently playing */
    cJSON *playing = cJSON_GetObjectItemCaseSensitive(json, "playing");
    if (playing == NULL) {
        mvprintw(2, 1, "In menu");
    } else {
        attron(COLOR_PAIR(7));
        mvprintw(2, 1, "In game");
        attroff(COLOR_PAIR(7));
    }

    /* Show last seen */
    cJSON* seen_at = cJSON_GetObjectItemCaseSensitive(json, "seenAt");
    if (seen_at == NULL) {
        error("Invalid JSON");
        return;
    }

    time_t t = (long)seen_at->valuedouble/1000; // convert ms -> sec

    struct tm lt;
    char res[20];

    localtime_r(&t, &lt);
    strftime(res, sizeof(res), "%F", &lt);
    mvprintw(2, 18,"Last seen: %s", res);

    /* Show ratings */
    cJSON* perfs = cJSON_GetObjectItemCaseSensitive(json, "perfs");
    
    move(4, 1);
    process_ratings(cJSON_GetObjectItemCaseSensitive(perfs, "blitz"), "Blitz  ", BLITZ_SYMBOL);
    move(5, 1);
    process_ratings(cJSON_GetObjectItemCaseSensitive(perfs, "bullet"), "Bullet ", BULLET_SYMBOL);
    move(6, 1);
    process_ratings(cJSON_GetObjectItemCaseSensitive(perfs, "correspondence"), "Mailing", CORRESPONDENCE_SYMBOL);
    move(7, 1);
    process_ratings(cJSON_GetObjectItemCaseSensitive(perfs, "classical"), "Classic", CLASSICAL_SYMBOL);
    move(8, 1);
    process_ratings(cJSON_GetObjectItemCaseSensitive(perfs, "rapid"), "Rapid  ", RAPID_SYMBOL);

    getch();
}