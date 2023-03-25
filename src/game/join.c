#include <cjson/cJSON.h>
#include <ncurses.h>

#include "../api.h"

#include "utils.h"
#include "game.h"

short join(char* api_key) {
    curs_set(0);

    WINDOW* win;
    win = create_subwin("Join");
    cJSON* json = get_request(api_key, "https://lichess.org/api/account/playing");
    cJSON* fail = cJSON_GetObjectItemCaseSensitive(json, "fail");
    if (fail->valueint == 1) {
        sw_error(win, "Invalid JSON!");
        return 0;
    }
    cJSON* now_playing = cJSON_GetObjectItemCaseSensitive(json, "nowPlaying");
    if (cJSON_GetArraySize(now_playing) > 1) {
        /* Show warning */
        sw_warning(win, "Multiple game in progress!");
    }
    cJSON* game_info = cJSON_GetArrayItem(now_playing, 0);
    cJSON* color = cJSON_GetObjectItemCaseSensitive(game_info, "color");
    cJSON* speed = cJSON_GetObjectItemCaseSensitive(game_info, "speed");
    cJSON* rated = cJSON_GetObjectItemCaseSensitive(game_info, "rated");

    cJSON* opponent = cJSON_GetObjectItemCaseSensitive(game_info, "opponent");
    cJSON* opponent_name = cJSON_GetObjectItemCaseSensitive(opponent, "username");

    mvwprintw(win, 1, 1, "Color: ");
    if (color->valuestring == "white") {
        wattron(win, COLOR_PAIR(9));
        wprintw(win, "White");
        wattroff(win, COLOR_PAIR(9));
    } else {
        wattron(win, COLOR_PAIR(10));
        wprintw(win, "Black");
        wattroff(win, COLOR_PAIR(10));
    }
    mvwprintw(win, 2, 1, "Speed: ");
    wattron(win, COLOR_PAIR(7));
    wprintw(win, "%s", speed->valuestring);
    wattroff(win, COLOR_PAIR(7));
    mvwprintw(win, 3, 1, "Is rated: ");
    if (cJSON_IsTrue(rated)) {
        wattron(win, COLOR_PAIR(7));
        wprintw(win, "true");
        wattroff(win, COLOR_PAIR(7));
    } else {
        wattron(win, COLOR_PAIR(5));
        wprintw(win, "false");
        wattroff(win, COLOR_PAIR(5));
    }
    mvwprintw(win, 4, 1, "Against:");
    wattron(win, COLOR_PAIR(6));
    mvwprintw(win, 5, 1, "  %s", opponent_name->valuestring);
    wattroff(win, COLOR_PAIR(6));
    mvwprintw(win, 7, 1, "Join [y/n]");

    wrefresh(win);
    if (getch() == 'y') {

        /* Cleanup */
        delwin(win);

        cJSON* game_id = cJSON_GetObjectItemCaseSensitive(game_info, "gameId");
        game_stream(api_key, game_id->valuestring);

        return 1;
    }

    /* Cleanup */
    delwin(win);
    touchwin(stdscr);
    wrefresh(stdscr);

    return 0;
}