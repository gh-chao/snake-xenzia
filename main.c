#include <ncurses.h>
#include "view_engine_ncurses.h"

int main() {
    // 初始化视图引擎
    view_engine_s view_engine;
    view_engine_ncursess_init(&view_engine);
    view_engine_ncursess_construct(&view_engine);

    // 创建游戏
    game_s snake_xenzia;
    // 初始化游戏
    game_construct(&snake_xenzia, &view_engine, getmaxx(stdscr), getmaxy(stdscr));
    // 游戏开始
    game_start(&snake_xenzia);
    // 游戏结束
    game_destruct(&snake_xenzia);

    // 视图引擎结束
    view_engine_ncursess_destruct(&view_engine);
    return 0;
}



