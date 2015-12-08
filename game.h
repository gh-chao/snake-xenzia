#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef SNAKE_XENZIA_GAME_H
#define SNAKE_XENZIA_GAME_H
#endif //SNAKE_XENZIA_GAME_H

// 方向
#define DIRECTION_UP    1
#define DIRECTION_DOWN  -1
#define DIRECTION_LEFT  2
#define DIRECTION_RIGHT -2

// 控制码
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

// 蛇
typedef struct snake {
    int direction;
    struct node *head;
} snake_s;

// 初始化
void snake_construct(snake_s *this, int x, int y);
// 销毁
void snake_destruct(snake_s *this);
// 前进
void snake_run(snake_s *this, int direction);
// 吃
void snake_eat(snake_s *this, food_s *food);
// 计算蛇头下一步x坐标
int snake_next_x(snake_s *this, int direction);
// 计算蛇头下一步y坐标
int snake_next_y(snake_s *this, int direction);

// 游戏
typedef struct game {
    // UI
    struct ui *ui;
    // 分数
    int score;
    // 步数
    int step;
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

// UI
typedef struct ui {
    // 场景范围
    int scene_x;
    int scene_y;
    // ui层定义变量
    void *properties;
    // 欢迎界面
    void (*welcome)(struct ui *ui, game_s *game);
    // 渲染游戏主体
    void (*render_body)(struct ui *ui,game_s *game);
    // 渲染游戏结束
    void (*game_over)(struct ui *ui,game_s *game);
    // 读取控制码
    int (*read_control)(struct ui *ui);
} ui_s;

// 游戏初始化
void game_construct(game_s *this, ui_s *ui);
// 游戏销毁
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