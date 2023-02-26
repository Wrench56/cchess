#include <stdio.h>
#include <string.h>

#include <ncurses.h>

#include "../settings.h"
#include "../cipher.h"

#include "dashboard.h"
#include "register.h"


void login() {
    int size = 0;
    
    /* Check if there is .autologin file */
    FILE *file;
    if ((file = fopen(".autologin", "r")))
    {
        /* Yes, it exists! Read it. */
        fseek(file, 0, SEEK_END); // Maybe I should just skip this part and only load in 24 chars...
        size = ftell(file) + 1;
        fseek(file, 0, SEEK_SET);

        char file_content[size];
        file_content[24] = '\0';
        char* token;

        fread(file_content, sizeof(file_content) - 1, size, file);
        token = xor_cipher(file_content, AUTOLOGIN_KEY, strlen(file_content), AUTOLOGIN_KEY_LEN);

        fclose(file);
        show_dashboard(token);

        return;
    }
    /* Continue with asking for username & password */

    char key;
    short c_xu;
    short c_xp;

    short c_y;
    short menu;
    
    char username[20];
    char password[50];
    
    /* ================== */
            login:
    /* ================== */

    mvprintw(0, 1, "Welcome to cchess!");
    mvprintw(2, 1, "Username:");
    mvprintw(3, 2, ">");
    mvprintw(5, 1, "Password:");
    mvprintw(6, 2, ">");
    mvprintw(10, 10, "Press CTRL+N to register!");

    c_xu = 0;
    c_xp = 0;
    c_y = 3;
    menu = 0;
    memset(username, '\0', sizeof(username));
    memset(password, '\0', sizeof(password));
    move(3, 3);
    while (1) {
        key = getch();
        if (key == '\n') {
            break;
        } else if (key == '\t') {
            if (menu == 0) {
                menu++;
                c_y = 6;
                move(c_y, c_xp+3);
            } else {
                menu--;
                c_y = 3;
                move(c_y, c_xu+3);
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
            } else {
                if (c_xp < 1) {
                    continue;
                }
                c_xp--;
                mvprintw(c_y, c_xp+3, " ");
                move(c_y, c_xp+3);
                password[c_xp] = '\0';
            }
        } else if (strcmp(unctrl(key), "^B") == 0) { // CTRL + B
            return;
        } else if (key > 31 && key < 127) {
            if (menu == 0) {
                mvprintw(3, c_xu+3, "%c", key);
                username[c_xu] = key;
                c_xu++;
            } else {
                mvprintw(6, c_xp+3, "*");
                password[c_xp] = key;
                c_xp++;
            }
        } else if (strcmp(unctrl(key), "^N") == 0) {
            clear();
            reg();
            clear();
            goto login;
        }
    }
    if (strlen(password) == 0)
    {
        clear();
        attron(COLOR_PAIR(5));
        mvprintw(8, 1, "Error: ");
        attroff(COLOR_PAIR(5));
        printw("Invalid password");
        goto login;
    }

    if (file = fopen(username, "r"))
    {
        /* Yes, user file exists! Read it. */

        size = 0;

        fseek(file, 0, SEEK_END);
        size = ftell(file) + 1;
        fseek(file, 0, SEEK_SET);

        char file_content[size];
        file_content[24] = '\0';
        char* token;

        fread(file_content, sizeof(file_content) - 1, size, file);
        token = xor_cipher(file_content, password, strlen(file_content), strlen(password));

        fclose(file);
        show_dashboard(token);
    }
    else
    {
        clear();
        attron(COLOR_PAIR(5));
        mvprintw(8, 1, "Error: ");
        attroff(COLOR_PAIR(5));
        printw("Invalid username");
        goto login;
    }
    



}