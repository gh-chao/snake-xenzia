#include <ncurses.h>
#include "game.h"

int main() {
    initscr();
    noecho();
    raw();
    keypad(stdscr, true);
    curs_set(0);
    // 创建游戏
    game_s snake_xenzia;
    // 初始化
    game_construct(&snake_xenzia, getmaxx(stdscr), getmaxy(stdscr));
    // 游戏开始
    game_start(&snake_xenzia);
    endwin();
    return 0;
}



