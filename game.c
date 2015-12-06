#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include "game.h"

// 游戏初始化
void game_construct(game_s *this, view_engine_s *view_engine, int scene_x, int scene_y) {
    // 视图引擎
    this->view_engine = view_engine;
    // 食物指针初始化
    this->food = NULL;
    // 设置场景大小
    this->scene_x = scene_x;
    this->scene_y = scene_y;
    // 默认前进方向
    this->snake_direction = DIRECTION_UP;
    // 初始化蛇
    this->snake = (snake_s *)malloc(sizeof(snake_s));
    snake_construct(this->snake, scene_x / 2, scene_y / 2);
}

// 游戏释放
void game_destruct(game_s *this) {
    snake_destruct(this->snake);
    free(this->snake);
    this->snake = NULL;
}

// 游戏启动
void game_start(game_s *this) {
    this->view_engine->render_welcome(this);
    this->view_engine->read_input_key_code();

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

    this->view_engine->read_input_key_code();
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
                this->view_engine->render_body(this);
                continue;
            }
        }
        // 蛇运动
        snake_run(this->snake, this->snake_direction);
        this->view_engine->render_body(this);
    }
}

void *game_thread_get_key_input(game_s *this) {
    while (TRUE) {
        int direction = 0;
        switch (this->view_engine->read_input_key_code()) {
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

void game_end(game_s *this) {
    // 结束
    if (this->pthread_t_feeding) {
        pthread_cancel(this->pthread_t_feeding);
    }
    if (this->pthread_t_update_direction) {
        pthread_cancel(this->pthread_t_update_direction);
    }
    if (this->pthread_t_run) {
        pthread_cancel(this->pthread_t_run);
    }
    this->view_engine->render_game_over(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void snake_construct(snake_s *this, int x, int y) {
    this->head = (node_s *) malloc(sizeof(node_s));
    this->head->x = x;
    this->head->y = y;
    this->head->next = NULL;
}

void snake_destruct(snake_s *this) {
    node_s *p = this->head;
    this->head = NULL;
    node_s *next;
    while (p != NULL) {
        next = p->next;
        p->next = NULL;
        free(p);
        p = next;
    }
}

void snake_run(snake_s *this, int direction) {
    this->direction = direction;
    int x = this->head->x;
    int y = this->head->y;
    int x2, y2;

    this->head->x = snake_next_x(this, direction);
    this->head->y = snake_next_y(this, direction);

    node_s *p = this->head->next;

    while (p != NULL) {
        x2 = p->x;
        y2 = p->y;
        p->x = x;
        p->y = y;
        x = x2;
        y = y2;
        p = p->next;
    }

}

void snake_eat(snake_s *this, food_s *food) {
    node_s * n = (node_s *)malloc(sizeof(node_s));
    n->x = food->x;
    n->y = food->y;
    n->next = this->head;
    this->head = n;
}

int snake_next_x(snake_s *this, int direction) {
    switch (direction) {
        case DIRECTION_LEFT:
            return this->head->x - 1;
        case DIRECTION_RIGHT:
            return this->head->x + 1;
        default:
            return this->head->x;
    }
}

int snake_next_y(snake_s *this, int direction) {
    switch (direction) {
        case DIRECTION_UP:
            return this->head->y - 1;
        case DIRECTION_DOWN:
            return this->head->y + 1;
        default:
            return this->head->y;
    }
}
