#include "food.h"

#ifndef SNAKE_XENZIA_SNAKE_H
#define SNAKE_XENZIA_SNAKE_H

#endif //SNAKE_XENZIA_SNAKE_H

// 方向
#define DIRECTION_UP    1
#define DIRECTION_DOWN  -1
#define DIRECTION_LEFT  2
#define DIRECTION_RIGHT -2

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