// coord_queue.h

#ifndef __COORD_QUEUE_H
#define __COORD_QUEUE_H

#include <stdint.h>

typedef struct {
    float x;
    float y;
} Point;

typedef struct CoordNode {
    Point data;
    struct CoordNode* next;
} CoordNode;

typedef struct {
    CoordNode* front;  // ��ͷ������
    CoordNode* rear;   // ��β�����
} CoordQueue;

// ��������
void init_coord_queue(CoordQueue* q);
void enqueue_coord(CoordQueue* q, float x, float y);
int dequeue_coord(CoordQueue* q, Point* out_point);
void free_coord_queue(CoordQueue* q);
int is_coord_queue_empty(CoordQueue* q);

extern CoordQueue test;

#endif
