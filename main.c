#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#define MAX_X getmaxx(stdscr)
#define MAX_Y getmaxy(stdscr)

void print_wall();
void welcome();
void run();




int main() {
    initscr();
    noecho();
    raw();
    curs_set(0);
    print_wall();

    welcome();
    refresh();
    getchar();

    endwin();
    return 0;
}

void print_wall() {
    border('*', '*', '*', '*', '*', '*', '*', '*');
}

void welcome() {
    char * welcomes = "welcome to snake xenzia";
    int x = (int) ((MAX_X - strlen(welcomes)) / 2 - 2);
    int y = MAX_Y / 2 - 2;
    mvprintw(y, x, welcomes);
}

void run() {

    struct node snake;



}

