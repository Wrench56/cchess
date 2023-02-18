#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

#include "../settings.h"
#include "home.h"

char* xor_cipher(char* data, char* key, int dataLen, int keyLen) {
	char* output = (char*)malloc(sizeof(char) * dataLen);

	for (int i = 0; i < dataLen; ++i) {
		output[i] = data[i] ^ key[i % keyLen];
	}

	return output;
}

void login() {
    int size;
    char file_content[size];
    char* token;
    
    /* Check if there is .autologin file */
    FILE *file;
    if ((file = fopen(".autologin", "r")))
    {
        /* Yes, it exists! Read it. */
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        fseek(file, 0, SEEK_SET);   
        fgets(file_content, size+1, file);

        token = xor_cipher(file_content, AUTOLOGIN_KEY, strlen(file_content), AUTOLOGIN_KEY_LEN);

        fclose(file);
        home(token);

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
        /* Yes, it exists! Read it. */

        size = 0;

        fseek(file, 0, SEEK_END);
        size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char file_content[size];
        char* token;    

        fgets(file_content, size+1, file);

        token = xor_cipher(file_content, password, strlen(file_content), strlen(password));

        fclose(file);
        home(token);
        
    } else {
        clear();
        attron(COLOR_PAIR(5));
        mvprintw(8, 1, "Error: ");
        attroff(COLOR_PAIR(5));
        printw("Invalid username");
        goto login;
    }
    



}