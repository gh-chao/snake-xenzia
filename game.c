#include <stdio.h>
#include "game.h"

// 游戏初始化
void game_construct(game_s *this, ui_s *ui) {
    // 视图引擎
    this->ui = ui;
    // 游戏检测
    if (this->ui->scene_x < 30 || this->ui->scene_y < 30) {
        printf("游戏场景不能小于30*30\n");
        exit(-1);
    }
    this->score = 0;
    this->step = 0;
    // 食物指针初始化
    this->food = NULL;
    // 默认前进方向
    this->snake_direction = DIRECTION_UP;
    // 初始化蛇
    this->snake = (snake_s *) malloc(sizeof(snake_s));
    snake_construct(this->snake, this->ui->scene_x / 2, this->ui->scene_y / 2);
}

// 游戏释放
void game_destruct(game_s *this) {
    snake_destruct(this->snake);
    free(this->snake);
    this->snake = NULL;
}

// 游戏启动
void game_start(game_s *this) {
    // 欢迎界面
    this->ui->welcome(this->ui, this);

    // 游戏控制
    pthread_create(&this->pthread_t_control, NULL, (void *(*)(void *)) game_thread_control, this);
    // 投食
    pthread_create(&this->pthread_t_feeding, NULL, (void *(*)(void *)) game_thread_feeding, this);
    // 运行
    pthread_create(&this->pthread_t_run, NULL, (void *(*)(void *)) game_thread_run, this);

    // 等待结束
    pthread_join(this->pthread_t_control, NULL);
    pthread_join(this->pthread_t_feeding, NULL);
    pthread_join(this->pthread_t_run, NULL);

    // 游戏结束界面
    this->ui->game_over(this->ui, this);
}

// 游戏运行
void *game_thread_run(game_s *this) {
    while (1) {
        for (int i = 0; i < 20; ++i) {
            pthread_testcancel();
            usleep(10000);
        }

        coordinate_s next;
        snake_next_coordinate(this->snake, &next, this->snake_direction);

        // 判断撞墙
        if (next.x == 0 || next.x == this->ui->scene_x || next.y == 0 || next.y == this->ui->scene_y) {
            game_over(this);
            return NULL;
        }

        // 判断吃到自己的尾巴
        node_s *p = this->snake->head;
        while (p != NULL) {
            if (next.x == p->coordinate->x && next.y == p->coordinate->y) {
                game_over(this);
                return NULL;
            }
            p = p->next;
        }
        // 存在食物
        if (this->food != NULL) {
            //判断是否可以吃到食物
            if (next.x == this->food->x && next.y == this->food->y) {
                // 吃掉食物
                snake_eat(this->snake, this->food);
                // 分数+1
                this->score += 10;
                // 把食物从场景中去掉
                free(this->food);
                this->food = NULL;
                this->ui->render_body(this->ui, this);
                continue;
            }
        }
        // 蛇运动
        snake_run(this->snake, this->snake_direction);
        this->ui->render_body(this->ui, this);
        // 步数加1
        this->step++;
    }
}

// 游戏控制
void *game_thread_control(game_s *this) {
    while (1) {
        int direction = 0;
        pthread_testcancel();
        switch (this->ui->read_control(this->ui)) {
            case CONTROL_UP:
                direction = DIRECTION_UP;
                break;
            case CONTROL_DOWN:
                direction = DIRECTION_DOWN;
                break;
            case CONTROL_LEFT:
                direction = DIRECTION_LEFT;
                break;
            case CONTROL_RIGHT:
                direction = DIRECTION_RIGHT;
                break;
            case CONTROL_QUIT:
                game_over(this);
                break;
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
    while (1) {
        // 五秒投食一次
        for (int i = 0; i < 500; ++i) {
            pthread_testcancel();
            usleep(10000);
        }
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
    int x = rand() % (this->ui->scene_x - 1) + 1;
    int y = rand() % (this->ui->scene_y - 1) + 1;

    // 判断次坐标是否跟蛇身冲突
    node_s *p = this->snake->head;
    while (NULL != p) {
        // 冲突
        if (x == p->coordinate->x && y == p->coordinate->y) {
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

void game_over(game_s *this) {
    // 关闭所有线程
    if (this->pthread_t_feeding) {
        pthread_cancel(this->pthread_t_feeding);
    }
    if (this->pthread_t_control) {
        pthread_cancel(this->pthread_t_control);
    }
    if (this->pthread_t_run) {
        pthread_cancel(this->pthread_t_run);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void snake_construct(snake_s *this, int x, int y) {
    this->head = (node_s *) malloc(sizeof(node_s));
    this->head->coordinate = (coordinate_s *) malloc(sizeof(coordinate_s));
    this->head->coordinate->x = x;
    this->head->coordinate->y = y;
    this->head->next = NULL;
}

void snake_destruct(snake_s *this) {
    node_s *p = this->head;
    this->head = NULL;
    node_s *next;
    while (p != NULL) {
        next = p->next;
        p->next = NULL;
        free(p->coordinate);
        free(p);
        p = next;
    }
}

// 蛇头前进
void snake_run(snake_s *this, int direction) {
    this->direction = direction;
    //当前节点新的坐标位置
    coordinate_s *coordinate = (coordinate_s *) malloc(sizeof(coordinate_s));
    snake_next_coordinate(this, coordinate, direction);

    coordinate_s *coordinate_tmp;

    node_s *p = this->head;
    while (p != NULL) {
        coordinate_tmp = p->coordinate;
        p->coordinate = coordinate;
        coordinate = coordinate_tmp;
        p = p->next;
    }
    // 最后的坐标释放掉
    free(coordinate);
}

void snake_eat(snake_s *this, food_s *food) {
    node_s *n = (node_s *) malloc(sizeof(node_s));
    n->coordinate = (coordinate_s *) malloc(sizeof(coordinate_s));
    n->coordinate->x = food->x;
    n->coordinate->y = food->y;
    n->next = this->head;
    this->head = n;
}

// 计算下一次蛇头的坐标
void snake_next_coordinate(snake_s *this, coordinate_s *coordinate, int direction) {
    coordinate->x = this->head->coordinate->x;
    coordinate->y = this->head->coordinate->y;
    switch (direction) {
        case DIRECTION_LEFT:
            coordinate->x--;
            break;
        case DIRECTION_RIGHT:
            coordinate->x++;
            break;
        case DIRECTION_UP:
            coordinate->y--;
            break;
        case DIRECTION_DOWN:
            coordinate->y++;
            break;
        default:
            break;
    }
}