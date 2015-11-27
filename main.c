#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "snake.h"

#define MAX_X getmaxx(stdscr)
#define MAX_Y getmaxy(stdscr)

void print_wall();

void welcome();

void run();

int main() {
    initscr();
    noecho();
    raw();
    keypad(stdscr, true);
    nodelay(stdscr, true);

    curs_set(0);
    print_wall();
    welcome();
    refresh();
    getchar();
    run();
    getchar();

    endwin();
    return 0;
}

void print_wall() {
    border('*', '*', '*', '*', '*', '*', '*', '*');
}

void welcome() {
    char *welcomes = "welcome to snake xenzia";
    int x = (int) ((MAX_X - strlen(welcomes)) / 2 - 2);
    int y = MAX_Y / 2 - 2;
    mvprintw(y, x, welcomes);
}

void run() {
    snake_t snake;
    snake_construct(&snake, MAX_X / 2, MAX_Y / 2);

    node_t *p = snake.head;
    while (NULL != p) {
        mvprintw(p->y, p->x, "+");
        p = p->next;
    }
    refresh();
    sleep(1);

    while (TRUE) {
        // 方向
        int key = getch();

        switch (key) {
            case KEY_UP:
            case 'w':
                if (snake.direction != DOWN) snake.direction = UP;
                break;
            case KEY_DOWN:
            case 's':
                if (snake.direction != UP) snake.direction = DOWN;
                break;
            case KEY_LEFT:
            case 'a':
                if (snake.direction != RIGHT) snake.direction = LEFT;
                break;
            case KEY_RIGHT:
            case 'd':
                if (snake.direction != LEFT) snake.direction = RIGHT;
                break;
            default:
                break;
        }

        snake_run(&snake);
        // 判断撞墙
        if (snake.head->x == 0 || snake.head->x == MAX_X || snake.head->y == 0 || snake.head->y == MAX_Y) {
            // 游戏结束
            break;
        }
        // 渲染脑袋
        mvprintw(snake.head->y, snake.head->x, "+");
        // 上次的尾巴抹掉
        if (snake.last_tail_x) {
            mvprintw(snake.last_tail_y, snake.last_tail_x, " ");
        }

        refresh();
        sleep(1);
    }
    snake_destruct(&snake);
}


