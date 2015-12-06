#include <pthread.h>

#include "snake.h"

#ifndef SNAKE_XENZIA_GAME_H
#define SNAKE_XENZIA_GAME_H

#endif //SNAKE_XENZIA_GAME_H



// 游戏
typedef struct game {
    // 场景范围
    int scene_x;
    int scene_y;
    // 线程句柄
    pthread_t pthread_t_update_direction;
    pthread_t pthread_t_feeding;
    pthread_t pthread_t_run;
    // 蛇的运动方向
    int snake_direction;
    // 食物
    struct food *food;
    // 蛇
    snake_s *snake;
} game_s;

// 游戏初始化
void game_construct(game_s *this, int scene_x, int scene_y);
// 游戏启动
void game_start(game_s *this);
// welcome
void game_welcome(game_s *this);
// 开始运行
void *game_thread_run(game_s *this);
// 监听键盘 线程
void *game_thread_get_key_input(game_s *this);
// 投食 线程
void *game_thread_feeding(game_s *this);
// 投食
void game_feeding(game_s *this);

// 渲染
void game_render(game_s *this);

void game_end(game_s *this);