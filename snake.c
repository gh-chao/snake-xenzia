#include <stdlib.h>
#include "snake.h"

void snake_construct(snake_t *this, int x, int y) {
    this->direction = UP;
    this->head = (node_t *) malloc(sizeof(node_t));
    this->head->x = x;
    this->head->y = y;
    this->tail = this->head;
}

void snake_run(snake_t *this) {
    node_t *p = this->head->next;
    int x = this->head->x;
    int y = this->head->y;

    // 记录现在尾巴的位置
    this->last_tail_x = this->tail->x;
    this->last_tail_y = this->tail->y;

    // 脑袋向前移动
    switch (this->direction) {
        case UP:
            this->head->y --;
            break;
        case DOWN:
            this->head->y ++;
            break;
        case LEFT:
            this->head->x --;
            break;
        case RIGHT:
            this->head->x ++;
            break;
        default:
            exit(-1);
    }

    // 身子向前移动
    int tmp;
    while (p != NULL) {
        tmp = p->x;
        p->x = x;
        x = tmp;
        tmp = p->y;
        p->y = y;
        y = tmp;
        p = p->next;
    }
}

void snake_destruct(snake_t *this) {
    node_t *current = this->head;
    node_t *next = current->next;
    this->head = NULL;

    while (NULL != current) {
        free(current);
        current = next;
        if (current != NULL) {
            next = current->next;
        }
    }

}
