#include <string.h>
#include <ncurses.h>
#include "ui_ncurses.h"

typedef struct {
    int max_x;
    int max_y;
} ui_ncurses_properties;

void ui_ncurses_init(ui_s *this) {
    this->welcome = &ui_ncurses_render_welcome;
    this->render_body = &ui_ncurses_render_body;
    this->game_over = &ui_ncurses_render_game_over;
    this->read_control = &ui_ncurses_read_control;
}

void ui_ncurses_construct(ui_s *this) {
    initscr();
    noecho();
    raw();
    keypad(stdscr, true);
    curs_set(0);

    this->properties = malloc(sizeof(ui_ncurses_properties));
    int max_x = getmaxx(stdscr) - 1;
    int max_y = getmaxy(stdscr) - 1;
    ((ui_ncurses_properties *) this->properties)->max_x = max_x;
    ((ui_ncurses_properties *) this->properties)->max_y = max_y;
    this->scene_x = max_x;
    this->scene_y = max_y - 2;
}

void ui_ncurses_destruct(ui_s *this) {
    free(this->properties);
    this->properties = NULL;
    endwin();
}

void ui_ncurses_render_welcome(ui_s *this, game_s *game) {
    clear();
    border('*', '*', '*', '*', '*', '*', '*', '*');
    char welcomes[] = "Snake Xenzia";
    int x = (int) ((this->scene_x - strlen(welcomes)) / 2 + 1);
    int y = this->scene_y / 2 - 2;
    mvprintw(y, x, welcomes);
    refresh();
    getch();
}

void ui_ncurses_render_body(ui_s *this, game_s *game) {
    clear();
    // 渲染墙体
    border('*', '*', '*', '*', '*', '*', '*', '*');
    for (int i = 0; i < this->scene_x; ++i) {
        mvprintw(this->scene_y, i, "*");
    }
    // 渲染蛇的身体
    node_s *node = game->snake->head;
    while (node != NULL) {
        mvprintw(node->coordinate->y, node->coordinate->x, "*");
        node = node->next;
    }
    //渲染食物
    if (game->food != NULL) {
        mvprintw(game->food->y, game->food->x, "+");
    }
    // 渲染底部状态栏
    char status_bar[28];
    sprintf(status_bar, "score: %d  step: %d", game->score, game->step);
    int x = (int) ((this->scene_x - strlen(status_bar)) / 2 + 1);
    int y = ((ui_ncurses_properties *) this->properties)->max_y - 1;
    mvprintw(y, x, status_bar);

    refresh();
}

void ui_ncurses_render_game_over(ui_s *this, game_s *game) {
    clear();
    border('*', '*', '*', '*', '*', '*', '*', '*');
    char welcomes[] = "Game Over";
    int x = (int) ((this->scene_x - strlen(welcomes)) / 2 + 1);
    int y = game->ui->scene_y / 2 - 2;
    mvprintw(y, x, welcomes);
    refresh();
    getch();
}

int ui_ncurses_read_control(ui_s *this) {
    int ch = getch();
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
