//
// Created by leo on 11/27/15.
//

#ifndef SNAKE_XENZIA_SNAKE_H
#define SNAKE_XENZIA_SNAKE_H
#endif //SNAKE_XENZIA_SNAKE_H

#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4

typedef struct snake {
    int direction;
    struct node *head;
} snake_t;

typedef struct node {
    int x;
    int y;
    struct node *next;
} node_t;

void snake_construct(snake_t *this);

void snake_run(snake_t *this);