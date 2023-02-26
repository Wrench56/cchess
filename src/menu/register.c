#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#include "cipher.h"

void reg() {
    char key;
    short c_xu;
    short c_xp;
    short c_xt;

    short c_y;
    short menu;
    
    char username[20];
    char password[50];
    char api_key[24];

    /* ================== */
            _register:
    /* ================== */

    mvprintw(0, 10, "** Register an account **");
    mvprintw(2, 1, "Username:");
    mvprintw(3, 2, ">");
    mvprintw(5, 1, "Password:");
    mvprintw(6, 2, ">");
    mvprintw(8, 1, "Token (max 24 chars):");
    mvprintw(9, 2, ">");

    c_xu = 0;
    c_xp = 0;
    c_xt = 0;
    c_y = 3;
    menu = 0;
    memset(username, '\0', sizeof(username));
    memset(password, '\0', sizeof(password));
    memset(api_key, '\0', sizeof(api_key));
    move(3, 3);
    while (1) {
        key = getch();
        if (key == '\n') {
            break;
        } else if (key == '\t') {
            if (menu < 2) {
                menu++;
                c_y += 3;
                
            } else {
                menu = 0;
                c_y = 3;
                move(c_y, c_xu+3);
            }
            if (menu == 0) {
                move(c_y, c_xu+3);
            } else if (menu == 1) {
                move(c_y, c_xp+3);
            } else {
                move(c_y, c_xt+3);
            }

        } else if (key == '\b' || key == 127 || key == KEY_BACKSPACE) {
            if (menu == 0) {
                if (c_xu < 1) {
                    continue;
                }
                c_xu--;
                mvprintw(c_y, c_xu+3, " ");
                move(c_y, c_xu+3);
                username[c_xu] = '\0';
            } else if (menu == 1) {
                if (c_xp < 1) {
                    continue;
                }
                c_xp--;
                mvprintw(c_y, c_xp+3, " ");
                move(c_y, c_xp+3);
                password[c_xp] = '\0';

            } else {
                if (c_xt < 1) {
                    continue;
                }
                c_xt--;
                mvprintw(c_y, c_xt+3, " ");
                move(c_y, c_xt+3);
                api_key[c_xt] = '\0';
            }
        } else if (strcmp(unctrl(key), "^B") == 0) { // CTRL + B
            return;
        } else if (key > 31 && key < 127) {
            if (menu == 0) {
                mvprintw(3, c_xu+3, "%c", key);
                username[c_xu] = key;
                c_xu++;
            } else if (menu == 1) {
                mvprintw(6, c_xp+3, "*");
                password[c_xp] = key;
                c_xp++;
            } else {
                mvprintw(9, c_xt+3, "%c", key);
                api_key[c_xt] = key;
                c_xt++;
            }
        }
    }

    FILE *file;
    if (file = fopen(username, "r"))
    {
        /* File exists! Error */

        fclose(file);
        clear();
        attron(COLOR_PAIR(5));
        mvprintw(10, 1, "Error: ");
        attroff(COLOR_PAIR(5));
        printw("User exists!");

        goto _register;
    }
    if (strlen(username) == 0 || strlen(password) == 0 || strlen(api_key) == 0) {
        clear();
        attron(COLOR_PAIR(5));
        mvprintw(10, 1, "Error: ");
        attroff(COLOR_PAIR(5));
        printw("Required field empty!");

        goto _register;
    }

    char* encrypted_token;
    encrypted_token = xor_cipher(api_key, password, strlen(api_key), strlen(password));

    file = fopen(username, "w");
    fputs(encrypted_token, file);
    fclose(file);
}