#pragma once

WINDOW* create_subwin(char* title);
void sw_error(WINDOW* win, char* message);
void sw_warning(WINDOW* win, char* message);