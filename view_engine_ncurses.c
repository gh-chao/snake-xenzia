#include <string.h>
#include <ncurses.h>
#include "view_engine_ncurses.h"

void view_engine_ncursess_init(view_engine_s *view_engine) {
    view_engine->render_welcome = &view_engine_ncursess_render_welcome;
    view_engine->render_body = &view_engine_ncursess_render_body;
    view_engine->render_game_over = &view_engine_ncursess_render_game_over;
    view_engine->read_control = &view_engine_ncursess_read_control;
}

void view_engine_ncursess_construct(view_engine_s *view_engine) {
    initscr();
    noecho();
    raw();
    keypad(stdscr, true);
    curs_set(0);
}

void view_engine_ncursess_destruct(view_engine_s *view_engine) {
    endwin();
}

void view_engine_ncursess_render_welcome(game_s *game) {
    clear();
    border('*', '*', '*', '*', '*', '*', '*', '*');
    char welcomes[] = "Snake Xenzia";
    int x = (int) ((game->scene_x - strlen(welcomes)) / 2 - 2);
    int y = game->scene_y / 2 - 2;
    mvprintw(y, x, welcomes);
    refresh();
    getch();
}

void view_engine_ncursess_render_body(game_s *game) {
    clear();
    // 渲染墙体
    border('*', '*', '*', '*', '*', '*', '*', '*');
    // 渲染蛇的身体
    node_s *p = game->snake->head;
    while (p != NULL) {
        mvprintw(p->y, p->x, "*");
        p = p->next;
    }
    //渲染食物
    if (game->food != NULL) {
        mvprintw(game->food->y, game->food->x, "+");
    }
    refresh();
}

void view_engine_ncursess_render_game_over(game_s *game) {
    clear();
    border('*', '*', '*', '*', '*', '*', '*', '*');
    char welcomes[] = "Game Over";
    int x = (int) ((game->scene_x - strlen(welcomes)) / 2 - 2);
    int y = game->scene_y / 2 - 2;
    mvprintw(y, x, welcomes);
    refresh();
    getch();
}

int view_engine_ncursess_read_control() {
    int ch =  getch();
    switch (ch) {
        case 'w':
        case KEY_UP:
            return CONTROL_UP;
        case 's':
        case KEY_DOWN:
            return CONTROL_DOWN;
        case 'a':
        case KEY_LEFT:
            return CONTROL_LEFT;
        case 'd':
        case KEY_RIGHT:
            return CONTROL_RIGHT;
        case 'q':
            return CONTROL_QUIT;
        default:
            return 0;
    }
}
