#include <stdlib.h>
#include "snake.h"

void snake_construct(snake_s *this, int x, int y) {
    this->head = (node_s *) malloc(sizeof(node_s));
    this->head->x = x;
    this->head->y = y;
    this->head->next = NULL;
}

void snake_destruct(snake_s *this) {
    node_s *p = this->head;
    node_s *next;
    while (p != NULL) {
        next = p->next;
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
