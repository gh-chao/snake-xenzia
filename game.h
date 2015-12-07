#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef SNAKE_XENZIA_GAME_H
#define SNAKE_XENZIA_GAME_H
#endif //SNAKE_XENZIA_GAME_H

// 方向
#define DIRECTION_UP    1
#define DIRECTION_DOWN  -1
#define DIRECTION_LEFT  2
#define DIRECTION_RIGHT -2

#define CONTROL_QUIT 1001
#define CONTROL_UP DIRECTION_UP
#define CONTROL_DOWN DIRECTION_DOWN
#define CONTROL_LEFT DIRECTION_LEFT
#define CONTROL_RIGHT DIRECTION_RIGHT

// 食物
typedef struct food {
    int x;
    int y;
} food_s;

// 身体节点
typedef struct node {
    int x;
    int y;
    struct node *next;
} node_s;

typedef struct snake {
    int direction;
    struct node *head;
} snake_s;

void snake_construct(snake_s *this, int x, int y);

void snake_destruct(snake_s *this);

void snake_run(snake_s *this, int direction);

void snake_eat(snake_s *this, food_s *food);

int snake_next_x(snake_s *this, int direction);

int snake_next_y(snake_s *this, int direction);

// 游戏
typedef struct game {
    struct view_engine *view_engine;
    // 场景范围
    int scene_x;
    int scene_y;
    // 线程句柄
    pthread_t pthread_t_control;
    pthread_t pthread_t_feeding;
    pthread_t pthread_t_run;
    // 蛇的运动方向
    int snake_direction;
    // 食物
    food_s *food;
    // 蛇
    snake_s *snake;
} game_s;

typedef struct view_engine {
    void (*render_welcome)(game_s *game);

    void (*render_body)(game_s *game);

    void (*render_game_over)(game_s *game);

    int (*read_control)();
} view_engine_s;

// 游戏初始化
void game_construct(game_s *this, view_engine_s *view_engine, int scene_x, int scene_y);

void game_destruct(game_s *this);

// 游戏启动
void game_start(game_s *this);

// 开始运行
void *game_thread_run(game_s *this);

// 游戏控制 线程
void *game_thread_control(game_s *this);

// 投食 线程
void *game_thread_feeding(game_s *this);

// 投食
void game_feeding(game_s *this);

// 游戏结束
void game_over(game_s *this);