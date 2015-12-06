//
// Created by leo on 15/12/6.
//

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

// 游戏初始化
void game_construct(game_s *this, int scene_x, int scene_y) {
    this->food = NULL;
    // 设置场景大小
    this->scene_x = scene_x;
    this->scene_y = scene_y;
    // 默认前进方向
    this->snake_direction = DIRECTION_UP;
    // 初始化蛇
    snake_construct(this->snake, scene_x / 2, scene_y / 2);
}

// 游戏启动
void game_start(game_s *this) {
    game_welcome(this);
    getch();
    // 监听键盘
    pthread_create(&this->pthread_t_update_direction, NULL, (void *(*)(void *)) game_thread_get_key_input, this);
    // 投食
    pthread_create(&this->pthread_t_feeding, NULL, (void *(*)(void *)) game_thread_feeding, this);
    // 运行
    pthread_create(&this->pthread_t_run, NULL, (void *(*)(void *)) game_thread_run, this);

    // 等待结束
    pthread_join(this->pthread_t_update_direction, NULL);
    pthread_join(this->pthread_t_feeding, NULL);
    pthread_join(this->pthread_t_run, NULL);
    getch();
}

// 游戏运行
void *game_thread_run(game_s *this) {
    while (TRUE) {
        usleep(500000);
        int next_x = snake_next_x(this->snake, this->snake_direction);
        int next_y = snake_next_y(this->snake, this->snake_direction);

        // 判断撞墙
        if (next_x == 0 || next_x == this->scene_x || next_y == 0 || next_y == this->scene_y) {
            game_end(this);
            return NULL;
        }

        // 判断吃到自己的尾巴
        node_s *p = this->snake->head;
        while (p != NULL) {
            if(next_x == p->x && next_y == p->y) {
                game_end(this);
                return NULL;
            }
            p = p->next;
        }

        // 存在食物
        if (this->food != NULL) {
            //判断是否可以吃到食物
            if (next_x == this->food->x && next_y == this->food->y) {
                // 吃掉食物
                snake_eat(this->snake, this->food);
                // 把食物从场景中去掉
                free(this->food);
                this->food = NULL;
                // 闪烁一下屏幕
                flash();
                game_render(this);
                continue;
            }
        }

        // 蛇运动
        snake_run(this->snake, this->snake_direction);
        game_render(this);
    }
}

void *game_thread_get_key_input(game_s *this) {
    while (TRUE) {
        int direction = 0;
        switch (getch()) {
            case 'w':
            case KEY_UP:
                direction = DIRECTION_UP;
                break;
            case 's':
            case KEY_DOWN:
                direction = DIRECTION_DOWN;
                break;
            case 'a':
            case KEY_LEFT:
                direction = DIRECTION_LEFT;
                break;
            case 'd':
            case KEY_RIGHT:
                direction = DIRECTION_RIGHT;
                break;
            case 'q':
                game_end(this);
            default:
                break;
        }
        if (direction + this->snake->direction != 0 && direction != 0) {
            this->snake_direction = direction;
        }
    }
    return NULL;
}

void *game_thread_feeding(game_s *this) {
    while (TRUE) {
        // 五秒投食一次
        usleep(5000000);
        game_feeding(this);
    }
    return NULL;
}

void game_feeding(game_s *this) {
    // 如果有剩余食物, 就不投食
    if (this->food != NULL) {
        return;
    }

    // 生成坐标
    int x = rand() % (this->scene_x - 1) + 1;
    int y = rand() % (this->scene_y - 1) + 1;

    // 判断次坐标是否跟蛇身冲突
    node_s *p = this->snake->head;
    while (NULL != p) {
        // 冲突
        if (x == p->x && y == p->y) {
            // 重新生成
            game_feeding(this);
            return;
        }
        p = p->next;
    }
    this->food = (food_s *) malloc(sizeof(food_s));
    this->food->x = x;
    this->food->y = y;
}


void game_render(game_s *this) {
    clear();
    border('*', '*', '*', '*', '*', '*', '*', '*');
    // 渲染蛇的身体
    node_s *p = this->snake->head;
    while (p != NULL) {
        mvprintw(p->y, p->x, "*");
        p = p->next;
    }
    //渲染食物
    if (this->food != NULL) {
        mvprintw(this->food->y, this->food->x, "+");
    }
    refresh();
}

void game_end(game_s *this) {
    if (this->pthread_t_feeding) {
        pthread_cancel(this->pthread_t_feeding);
    }
    if (this->pthread_t_update_direction) {
        pthread_cancel(this->pthread_t_update_direction);
    }
    if (this->pthread_t_run) {
        pthread_cancel(this->pthread_t_run);
    }
    clear();
    char *welcomes = "Game Over";
    int x = (int) ((this->scene_x - strlen(welcomes)) / 2 - 2);
    int y = this->scene_y / 2 - 2;
    mvprintw(y, x, welcomes);
    refresh();
}

void game_welcome(game_s *this) {
    clear();
    char *welcomes = "Snake Xenzia";
    int x = (int) ((this->scene_x - strlen(welcomes)) / 2 - 2);
    int y = this->scene_y / 2 - 2;
    mvprintw(y, x, welcomes);
    refresh();
}
